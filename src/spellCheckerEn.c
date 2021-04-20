# include "../include/spellCheckerEn.h"
# include <libmorph/eng/include/mlma1033.h>

struct morphomodule morphoEng =
{
  mlmaenCheckWord,
  mlmaenLemmatize,
  mlmaenBuildForm,
  mlmaenCheckHelp
};  

int   spellCheckWordEn( const char*           pszstr )
{
  return spellCheckWord( &morphoEng, pszstr );
}

int   spellCheckHelpEn( char*                 szbuff,
                        unsigned              ccbuff,
                        const char*           pszstr )
{
  return spellCheckHelp( &morphoEng, szbuff, ccbuff, pszstr );
}
