/**
 *	Tecgraf's code with modifications by Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Oct 2012
 *
 *  OpenGL Error utility class.
 */

#ifndef _GL_ERROR_H_
#define _GL_ERROR_H_

#include <GL/glew.h>
#define PrintGLError() GLError::printGLError(__FILE__, __LINE__)
#define PrintGLErrorLabelHere(l) GLError::printGLError((l), __FILE__, __LINE__)
#define PrintGLErrorFunction() GLError::printGLError(__FUNCTION__)
#define PrintGLErrorLabel(l) GLError::printGLError((l))


class GLError
{
public:

  GLError();

  static const char* getGLErrorString (GLenum errCode);

  static bool printGLError(const char* label);
  static bool printGLError(const char* label, const char *file, int line);
  static bool printGLError(const char *file, int line);

};

#endif