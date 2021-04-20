# if !defined( __spellChecker_h__ )
# define  __spellChecker_h__
# include <libmorph/rus/include/mlma1049.h>

struct  morphomodule
{
  short (MLMA_API EXPORT *checkword)( const char*, unsigned short );
  short (MLMA_API EXPORT *lemmatize)( const char*, unsigned short,
                                      char*,
                                      lexeme_t*,
                                      char*,
                                      unsigned short,
                                      unsigned short,
                                      unsigned short );
  short (MLMA_API EXPORT *buildform)( const char*,
                                      lexeme_t,
                                      unsigned short,
                                      unsigned char,
                                      char*,
                                      unsigned short );
  short (MLMA_API EXPORT *checkhelp)( const char*,
                                      char* );
};

# if defined( __cplusplus )
extern "C" {
# endif  // __cplusplus

int   spellCheckWord( struct morphomodule*  module,
                      const char*           pszstr );
int   spellCheckHelp( struct morphomodule*  module,
                      char*                 szbuff,
                      unsigned              ccbuff,
                      const char*           pszstr );

# if defined( __cplusplus )
}
# endif  // __cplusplus

# endif  // __spellChecker_h__
