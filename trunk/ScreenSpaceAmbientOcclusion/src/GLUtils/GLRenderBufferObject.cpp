/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Mar 2010  
 *
 *  OpenGL RenderBufferObject utility class.
 */

#include <GL/glew.h>
#include <iostream>
#include "assert.h"

#include "GLRenderBufferObject.h"

using namespace std;

GLRenderBufferObject::GLRenderBufferObject(int width, int height)
:m_width(width)
,m_height(height)
{
   m_supported = true;
   GLenum err = glewInit();
   if (GLEW_OK != err)
   {
      m_supported = false;
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
   }

   if (!glewIsSupported("GL_VERSION_2_0"))
   {
      m_supported = false;
      printf("OpenGL 2.0 not supported\n");
   }

   if(!(glGenFramebuffersEXT && glDeleteFramebuffersEXT && glBindFramebufferEXT && glCheckFramebufferStatusEXT &&
      glGetFramebufferAttachmentParameterivEXT && glGenerateMipmapEXT && glFramebufferTexture2DEXT &&
      glFramebufferRenderbufferEXT && glGenRenderbuffersEXT && glDeleteRenderbuffersEXT &&
      glBindRenderbufferEXT && glRenderbufferStorageEXT && glGetRenderbufferParameterivEXT && glIsRenderbufferEXT))
   {
      m_supported = false;
      cout << "Video card does NOT support GL_EXT_framebuffer_object." << endl;
   }

   assert(m_supported);

   glGenRenderbuffersEXT(1, &m_rboId);
   glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_rboId);
   glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, m_width, m_height);
   glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
}

GLRenderBufferObject :: ~GLRenderBufferObject()
{
   if(m_supported)
      glDeleteRenderbuffersEXT(1, &m_rboId);
}

GLuint GLRenderBufferObject :: getId()
{
   return m_rboId;
}
