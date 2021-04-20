# include "../include/spellCheckerUa.h"
# include <libmorph/ukr/include/mlma1058.h>

struct morphomodule morphoUkr =
{
  mlmaukCheckWord,
  mlmaukLemmatize,
  mlmaukBuildForm,
  mlmaukCheckHelp
};  

int   spellCheckWordUa( const char*           pszstr )
{
  return spellCheckWord( &morphoUkr, pszstr );
}

int   spellCheckHelpUa( char*                 szbuff,
                        unsigned              ccbuff,
                        const char*           pszstr )
{
  return spellCheckHelp( &morphoUkr, szbuff, ccbuff, pszstr );
}
