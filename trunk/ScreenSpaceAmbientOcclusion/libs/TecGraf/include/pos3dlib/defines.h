
#ifndef POS3DLIB_DEFINES_H
#define POS3DLIB_DEFINES_H

#ifndef POS3DLIB_DLL
# ifndef TEC_DLL
#  define POS3DLIB_API
# else
#  define POS3DLIB_API __declspec(dllimport)
# endif
#else
# define POS3DLIB_API __declspec(dllexport)
#endif

#endif
