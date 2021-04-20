# if !defined( __spellCheckerEn_h__ )
# define  __spellCheckerEn_h__
# include "spellChecker.h"

# if defined( __cplusplus )
extern "C" {
# endif  // __cplusplus

int   spellCheckWordEn( const char*           pszstr );
int   spellCheckHelpEn( char*                 szbuff,
                        unsigned              ccbuff,
                        const char*           pszstr );

# if defined( __cplusplus )
}
# endif  // __cplusplus

# endif  // __spellCheckerEn_h__
