#ifndef UGL_ERROR_H
#define UGL_ERROR_H

#include "defines.h"

#ifdef __cplusplus
extern "C" {
#endif

UGL_API int ugl_printglerrorline (const char *label, const char *file, int line);
UGL_API int ugl_printglerrormsg (const char *label, const char *msg);
UGL_API void ugl_printglstacksstatus (const char *label);
UGL_API const char* ugl_errorstring (GLenum errCode);
  
#define ugl_printglerror(l) ugl_printglerrorline((l),__FILE__,__LINE__)

#ifdef __cplusplus
}
#endif

#endif
