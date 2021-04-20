# include "../include/spellChecker.h"
# include <string.h>
# include <assert.h>

struct  spellinghelp
{
  char*       output;
  unsigned    cchout;
  int         ncount;
};

int   helpCheckString( struct morphomodule* module,
                       const char*          pszstr,
                       unsigned             cchstr,
                       unsigned             dwsets )
{
  char  szword[0x100];

  if ( pszstr[cchstr] != '\0' )
  {
    if ( cchstr >= sizeof(szword) )
      return 0;
    pszstr = (char*)memcpy( szword, pszstr, cchstr );
      szword[cchstr] = '\0';
  }
  return module->checkword( pszstr, (unsigned short)dwsets ) > 0;
}

/********************************************************************************/
/* the string is graphically-exact; the only possible problem is the incorrect  */
/* capitalization; register the string                                          */
/********************************************************************************/
int   helpRegisterStr( struct morphomodule* module,
                       struct spellinghelp* lphelp,
                       const char*          pszstr,
                       unsigned             cchstr )
{
/* ensure string length is defined      */
  if ( cchstr == (unsigned)-1 || cchstr == 0 )
    cchstr = strlen( pszstr );

/* check for the buffer overflow        */
  if ( cchstr >= lphelp->cchout )
    return -1;

/* copy the string to the output buffer */
  memcpy( lphelp->output, pszstr, cchstr );
    lphelp->output[cchstr] = '\0';

/* check if the string is spelled ok    */
  if ( module->checkword( lphelp->output, 0 ) <= 0 )
  {
    lexeme_t  nlexid[32];
    char      szlemm[256];
    char      sginfo[256];

  /* lemmatize the word and get lexeme  */
    if ( module->lemmatize( lphelp->output, sfIgnoreCapitals, NULL, nlexid,
      sginfo, 0, sizeof(szlemm) / sizeof(szlemm[0]), sizeof(sginfo) ) <= 0 )
        return -1;

  /* restore minimal capitalization     */
    if ( module->buildform( NULL, nlexid[0], 0, ((SGramInfo*)(sginfo + 1))->iForm,
      szlemm, sizeof(szlemm) - 1 ) <= 0 )
        return -1;

  /* ensure the form length is the same */
    assert( strlen( szlemm ) == cchstr );

  /* copy source form                   */
    memcpy( lphelp->output, szlemm, cchstr );
  }
  lphelp->cchout -= cchstr + 1;
  lphelp->output += cchstr + 1;
  lphelp->ncount++;
  return 0;
}

int   helpFindMistype( struct morphomodule* module,
                       struct spellinghelp* lphelp,
                       const char*          source,
                       unsigned             srclen )
{
  char    szword[0x80];   /* the string template storage buffer       */
  char    szhelp[0x30];   /* the local buffer for toolhelpers output  */
  char*   srcend;
  int     rcount;
  char    chsave;
  int     fcount;

  /* check string length              */
  if ( srclen == (unsigned)-1 || srclen == 0 )
    srclen = strlen( source );

  /* create string copy               */
  memcpy( szword, source, srclen );
    szword[srclen] = '\0';

  for ( srcend = szword + srclen - 1, rcount = 0; srcend >= szword; --srcend )
  {
  /* replace next character and check */
    chsave = *srcend;
      *srcend = '?';

  /* check if any variants exist      */
  /* skip if nothing                  */
    if ( (fcount = module->checkhelp( szword, szhelp )) <= 0 )
    {
      *srcend = chsave;
      continue;
    }

  /* enumerate all the helpers and replace */
  /* the wildcard with the list characer   */
    while ( fcount-- > 0 )
      if ( szhelp[fcount] != '\0' )
      {
        *srcend = szhelp[fcount];

      /* formal check word                    */
        if ( module->checkword( szword, sfIgnoreCapitals ) <= 0 )
          continue;

      /* register the helper                  */
        if ( helpRegisterStr( module, lphelp, szword, srclen ) != 0 )
          return -1;
      }
  /* restore original transcription   */
    *srcend = chsave;
  }
  return 0;
}

int   helpFindRandoms( struct morphomodule* module,
                       struct spellinghelp* lphelp,
                       const char*          source,
                       unsigned             srclen )
{
  char        szword[0x80];   /* the string template storage buffer       */
  const char* ptrtop;
  const char* ptrend;
  char*       ptrout;

/* check source length                */
  if ( srclen == (unsigned)-1 || srclen == 0 )
    srclen = strlen( source );

/* check if word is too short         */
  if ( srclen < 2 )
    return 0;

/* check if last symbol random        */
  if ( helpCheckString( module, source, srclen - 1, sfIgnoreCapitals ) )
    if ( helpRegisterStr( module, lphelp, source, srclen - 1 ) != 0 )
      return -1;

/* check if first symbol random       */
  if ( helpCheckString( module, source + 1, srclen - 1, sfIgnoreCapitals ) )
    if ( helpRegisterStr( module, lphelp, source + 1, srclen - 1 ) != 0 )
      return -1;

/* create local copies without random */
  if ( srclen < 3 )
    return 0;

  ptrout = szword;
  ptrtop = source;
  ptrend = source + srclen;
    szword[srclen - 1] = '\0';
  *ptrout++ = *ptrtop++;
    memcpy( ptrout, ptrtop + 1, ptrend - ptrtop - 1 );
  while ( ptrtop < ptrend - 1 )
  {
  /* check the next helper word           */
    if ( helpCheckString( module, szword, srclen - 1, sfIgnoreCapitals ) )
      if ( helpRegisterStr( module, lphelp, szword, srclen - 1 ) != 0 )
        return -1;
    *ptrout++ = *ptrtop++;
  }
  return 0;
}

int   helpFindSwapped( struct morphomodule* module,
                       struct spellinghelp* lphelp,
                       const char*          source,
                       unsigned             srclen )
{
  char    szword[0x80];   /* the string template storage buffer       */
  char*   ptrtop;
  char*   ptrend;
  char    chswap;

/* check source length                */
  if ( srclen == (unsigned)-1 || srclen == 0 )
    srclen = strlen( source );

/* check if word is too short         */
  if ( srclen < 2 )
    return 0;

/* fill output buffer                 */
  ptrend = (ptrtop = (char*)memcpy( szword, source, srclen )) + srclen;

  for ( szword[srclen] = '\0'; ptrtop < ptrend - 1; ptrtop++ )
  {
    chswap = ptrtop[0];
      ptrtop[0] = ptrtop[1];
    ptrtop[1] = chswap;

  /* check the next helper word           */
    if ( helpCheckString( module, szword, srclen, sfIgnoreCapitals ) )
      if ( helpRegisterStr( module, lphelp, szword, srclen ) != 0 )
        return -1;

    chswap = ptrtop[0];
      ptrtop[0] = ptrtop[1];
    ptrtop[1] = chswap;
  }
  return 0;
}

int   helpFindSkipped( struct morphomodule* module,
                       struct spellinghelp* lphelp,
                       const char*          source,
                       unsigned             srclen )
{
  char    szword[0x80];   /* the string template storage buffer       */
  char    szhelp[0x20];
  char*   ptrtop;
  char*   ptrend;
  int     fcount;
  char    chswap;

/* check source length                */
  if ( srclen == (unsigned)-1 || srclen == 0 )
    srclen = strlen( source );

/* create local space-reserved copy   */
  ptrend = (ptrtop = (char*)memcpy( szword, source, srclen )) + srclen;
    ptrend[0] = '?';
    ptrend[1] = '\0';

/* run swap procedure right-to-left   */
  for ( ; ; --ptrend )
  {
  /* get helper characters              */
    if ( (fcount = module->checkhelp( szword, szhelp )) > 0 )
      while ( fcount-- > 0 )
        if ( szhelp[fcount] != '\0' )
        {
          *ptrend = szhelp[fcount];

        /* check the next helper word           */
          if ( helpCheckString( module, szword, srclen + 1, sfIgnoreCapitals ) )
            if ( helpRegisterStr( module, lphelp, szword, srclen + 1 ) != 0 )
              return -1;

          *ptrend = '?';
        }

  /* check if nothing to do             */
    if ( ptrend == ptrtop )
      break;
  /* swap the template                  */
    chswap = ptrend[-1];
      ptrend[-1] = ptrend[0];
    ptrend[0] = chswap;
  }
  return 0;
}

int   helpFindNoSpace( struct morphomodule* module,
                       struct spellinghelp* lphelp,
                       const char*          source,
                       unsigned             srclen )
{
  char      szword[0x80];   /* the string template storage buffer       */
  unsigned  srcpos;

/* check source length                */
  if ( srclen == (unsigned)-1 || srclen == 0 )
    srclen = strlen( source );

/* check no-space words         */
  for ( srcpos = 1; srcpos < srclen; srcpos++ )
    if ( helpCheckString( module, source, srcpos, 0 ) && helpCheckString(
      module, source + srcpos, srclen - srcpos, 0 ) )
    {
      memcpy( szword, source, srcpos );
        szword[srcpos] = ' ';
      memcpy( szword + srcpos + 1, source + srcpos, srclen - srcpos );
        szword[srclen + 1] = '\0';
      if ( helpRegisterStr( module, lphelp, szword, srclen + 1 ) != 0 )
        return -1;
    }
  return 0;
}

typedef int   (*helpfunc)( struct morphomodule*, struct spellinghelp*, const char*, unsigned );

helpfunc  helpfunclist[] =
{
  helpFindMistype,
  helpFindRandoms,
  helpFindSwapped,
  helpFindSkipped,
  helpFindNoSpace,
  NULL
};

int   spellCheckWord( struct morphomodule*  module,
                      const char*           pszstr )
{
  return module->checkword( pszstr, 0 );
}

int   spellCheckHelp( struct morphomodule*  module,
                      char*                 szbuff,
                      unsigned              ccbuff,
                      const char*           pszstr )
{
  struct spellinghelp sthelp = { szbuff, ccbuff, 0 };
  helpfunc*           lplist = helpfunclist;
  unsigned            cchstr = strlen( pszstr );

  while ( *lplist != NULL )
    if ( (*lplist++)( module, &sthelp, pszstr, cchstr ) != 0 )
      return -1;

  return sthelp.ncount;
}
