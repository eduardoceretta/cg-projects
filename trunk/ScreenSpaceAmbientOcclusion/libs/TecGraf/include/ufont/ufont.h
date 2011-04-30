// OpenGL Abstraction for font text
// Tecgraf/PUC-Rio
// cpalomo@tecgraf.puc-rio.br
// May 2010

#ifndef UFONT_UFONT_H
#define UFONT_UFONT_H

#include "defines.h"

/**
 * Static initializer: must be called at least once.
 * @return success of failure in initialization.
*/
UFONT_API bool UFontInit ();

/**
 * Add path to folder with fonts.
 */
UFONT_API void UFontAddFontsPath (const char* path);

#endif  // UFONT_UFONT_H
