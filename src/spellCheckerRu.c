# include "../include/spellCheckerRu.h"
# include <libmorph/rus/include/mlma1049.h>

struct morphomodule morphoRus =
{
  mlmaruCheckWord,
  mlmaruLemmatize,
  mlmaruBuildForm,
  mlmaruCheckHelp
};  

int   spellCheckWordRu( const char*           pszstr )
{
  return spellCheckWord( &morphoRus, pszstr );
}

int   spellCheckHelpRu( char*                 szbuff,
                        unsigned              ccbuff,
                        const char*           pszstr )
{
  return spellCheckHelp( &morphoRus, szbuff, ccbuff, pszstr );
}
