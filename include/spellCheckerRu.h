# if !defined( __spellCheckerRu_h__ )
# define  __spellCheckerRu_h__
# include "spellChecker.h"

# if defined( __cplusplus )
extern "C" {
# endif  // __cplusplus

int   spellCheckWordRu( const char*           pszstr );
int   spellCheckHelpRu( char*                 szbuff,
                        unsigned              ccbuff,
                        const char*           pszstr );

# if defined( __cplusplus )
}
# endif  // __cplusplus

# endif  // __spellCheckerRu_h__
