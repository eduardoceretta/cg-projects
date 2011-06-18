/**
 *	Eduardo Ceretta Dalla Favera with modifications of Cesar Tadeu Pozzer
 *  eduardo.ceretta@gmail.com, pozzer3@gmail.com
 *  Mar 2010  
 *
 *  OpenGL FrameBufferObject utility class.
 */

#ifndef _GL_FRAMEBUFFEROBJECT_H_
#define _GL_FRAMEBUFFEROBJECT_H_

#include <GL/glew.h>
#include <GL/glut.h>

#include <vector>

#include "GLUtils\GLRenderBufferObject.h"
using namespace std;

/**
 * Auxiliary FBO buffer struct. 
 *  Holds OpenGL memory buffer information.
 */
typedef struct bufferType{
  /**
   * Type of buffer bound to the FBO
   */
  enum BindType {Texture, RenderBufferObject, NotBind};

  /**
   * Current buffer bind type
   */
  BindType bindType;

  /**
   * Buffer id
   */
  GLuint id;

  /**
   * FrameBuffer Target
   *  (GL_COLOR_ATTACHMENT0_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_STENCIL_ATTACHMENT_EXT)
   */
  GLenum bufferTarget;

  /**
   * Simple Constructor
   */
  bufferType();

  /**
   * Bind the buffer to the FBO
   */
  void bind();
}GL_FBOBufferType;


/**
 * OpenGL FrameBufferObject utility class.
 */
class GLFrameBufferObject
{
  /**
  * Indicates if the frame buffer object is supported by the hardware.
  */ 
  bool m_supported;

  /**
   * Indicates if the FBO is active.
   */
  bool m_active;

  /**
   * OpenGL FBO id.
   */
  GLuint m_fboId;

  /**
   * FrameBuffer Width
   */
  int m_width;

  /**
   * FrameBuffer Height
   */
  int m_height;

  /**
   * Maximum number of ColorAttachments supported by the hardware
   */
  GLint m_maxColorAttachments;

  /**
   * Number of buffers in the current FBO
   */
  int m_numBuffers;

  /**
   * Array of buffers to draw
   *  Size = m_maxColorAttachments + 2
   */
  GLenum *m_drawBuffers;

  /**
   * Color Buffers Array.
   *  Size = m_maxColorAttachments
   */
  GL_FBOBufferType *m_colorBuffers;  

  /**
   * Depth Buffer 
   */
  GL_FBOBufferType m_dephtBuffer;

  /**
   * Stencil Buffer
   */
  GL_FBOBufferType m_stencilBuffer;

  /**
   * Array of RenderBufferObjects
   */
  vector<GLRenderBufferObject*> m_renderBuffers;

public:
  /**
   * Simple Constructor
   */
  GLFrameBufferObject();

  /**
   * Creates and initializes an FBO
   */
  GLFrameBufferObject(int width, int height);

  /**
   * Deletes allocated buffers in the GPU and CPU's memory
   */
  ~GLFrameBufferObject();

  /**
   * Attach Buffer to respective Color Buffer index.
   *  BindType: Texture, RenderBufferObject
   *  If id not given creates the buffer of BindType
   */
  GLuint attachToColorBuffer(GL_FBOBufferType::BindType bt, int index = -1, GLuint id = -1);

  /**
   * Attach Buffer to DepthBuffer.
   *  BindType: Texture, RenderBufferObject
   *  If id not given creates the buffer of BindType
   */
  GLuint attachToDepthBuffer(GL_FBOBufferType::BindType bt, GLuint id = -1);

  /**
   * Attach Buffer to DepthBuffer.
   *  BindType: Texture, RenderBufferObject
   *  If id not given creates the buffer of BindType
   */
  GLuint attachToStencilBuffer(GL_FBOBufferType::BindType bt, GLuint id = -1);

  /**
   * Set the FBO Active/Inactive
   */
  void setActive(bool);

  /**
   * Return if the FBO is active
   */
  bool isActive();

private:
  /**
   * Create a Texture2D of GL_RGBA32F_ARB , GL_RGBA, GL_FLOAT
   *  Texture size is widthXheight.
   */
  GLuint createTextureToRender(int width, int height);

  /**
   * Create a RenderBufferObject.
   *  RBO size is widthXheight.
   */
  GLuint createRenderBufferObjectToRender(int width, int height);

  /**
   * Check FBO satus.
   */
  bool checkFramebufferStatus();
};

#endif


