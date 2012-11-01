/**
 *	Tecgraf's code with modifications by Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Oct 2012
 *
 *  OpenGL Error utility class.
 */
#include <stdio.h>
#include <stdarg.h>

#include "GLError.h"

GLError::GLError ()
{

}

bool GLError::printGLError (const char* label)
{
  GLenum e = glGetError();
  if (e != GL_NONE)
  {
    printf("\n=GL_ERROR=\n");
    printf("Error: %s %s\n", getGLErrorString(e), label);
    printf("-GL_ERROR-\n\n");
    return false;
  }
  return true;
}

bool GLError::printGLError (const char* label, const char *file, int line)
{
  GLenum e = glGetError();
  if (e != GL_NONE)
  {
    printf("\n=GL_ERROR=\n");
    printf("Error:%s:%d: %s %s\n", file, line, getGLErrorString(e), label);
    printf("-GL_ERROR-\n\n");
    return false;
  }
  return true;
}

bool GLError::printGLError (const char *file, int line)
{
  GLenum e = glGetError();
  if (e != GL_NONE)
  {
    printf("\n=GL_ERROR=\n");
    printf("Error:%s:%d: %s\n", file, line, getGLErrorString(e));
    printf("-GL_ERROR-\n\n");
    return false;
  }
  return true;
}

const char* GLError::getGLErrorString (GLenum errCode)
{
  switch(errCode)
  {
  case GL_NO_ERROR:
    return "no error";
  case GL_INVALID_ENUM:
    return "invalid enum";
  case GL_INVALID_VALUE:
    return "invalid value";
  case GL_INVALID_OPERATION:
    return "invalid operation";
  case GL_STACK_OVERFLOW:
    return "stack overflow";
  case GL_STACK_UNDERFLOW:
    return "stack underflow";
  case GL_OUT_OF_MEMORY:
    return "out of memory";
  case GL_INVALID_FRAMEBUFFER_OPERATION_EXT:
    return "invalid framebuffer operation";
  default:
    return "(unknown error)";
  }
}
