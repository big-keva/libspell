# if !defined( __spellCheckerUa_h__ )
# define  __spellCheckerUa_h__
# include "spellChecker.h"

# if defined( __cplusplus )
extern "C" {
# endif  // __cplusplus

int   spellCheckWordUa( const char*           pszstr );
int   spellCheckHelpUa( char*                 szbuff,
                        unsigned              ccbuff,
                        const char*           pszstr );

# if defined( __cplusplus )
}
# endif  // __cplusplus

# endif  // __spellCheckerUa_h__
