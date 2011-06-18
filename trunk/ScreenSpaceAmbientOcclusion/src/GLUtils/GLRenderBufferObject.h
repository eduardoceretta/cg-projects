/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Mar 2010  
 *
 *  OpenGL RenderBufferObject utility class.
 */

#ifndef _GL_RENDERBUFFEROBJECT_H_
#define _GL_RENDERBUFFEROBJECT_H_

#include <GL/glew.h>
#include <GL/glut.h>


class GLRenderBufferObject
{
  /**
   * Indicates if the render buffer object is supported by the hardware.
   */ 
  bool m_supported;

  /**
   * OpenGL RenderBufferObject ID.
   */
  GLuint m_rboId;

  /**
   * RenderBuffer Width
   */
  int m_width;

  /**
   * RenderBuffer Height
   */
  int m_height;
public:

  /**
   * Constructs a RenderBufferObject if supported.
   */
  GLRenderBufferObject(int width, int height);

  /**
   * Deletes RenderBuffer from GPU's memory.
   */
  ~GLRenderBufferObject();

  /**
   * Gets the RenderBuffer ID.
   */
  GLuint getId();
};

#endif


