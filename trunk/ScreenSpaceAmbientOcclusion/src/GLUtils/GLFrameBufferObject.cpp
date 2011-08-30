/**
 *	Eduardo Ceretta Dalla Favera with modifications of Cesar Tadeu Pozzer
 *  eduardo.ceretta@gmail.com, pozzer3@gmail.com
 *  Mar 2010  
 *
 *  OpenGL FrameBufferObject utility class.
 */
#include <GL/glew.h>
#include <iostream>
#include <assert.h>
#include "GLFrameBufferObject.h"
#include "GLUtils/GLRenderBufferObject.h"
#include "GLUtils/GLTextureObject.h"

using namespace std;

/************************************************************************/
/* GL_FBOBufferType                                                     */
/************************************************************************/
bufferType::bufferType() 
:bindType(NotBind)
,bufferTarget(GL_COLOR_ATTACHMENT0_EXT)
{
}

void bufferType::bind()
{
  switch(bindType)
  {
    case Texture:
      glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, bufferTarget, GL_TEXTURE_2D, id, 0);
      break;
    case RenderBufferObject:
      glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, bufferTarget, GL_RENDERBUFFER_EXT, id);
      break;

    case NotBind:
    default:
      break;
  }
}



/************************************************************************/
/* GLFrameBufferObject                                                  */
/************************************************************************/
GLFrameBufferObject::GLFrameBufferObject()
:m_drawBuffers(NULL)
,m_colorBuffers(NULL)
{}


GLFrameBufferObject::GLFrameBufferObject(int width, int height)
:m_active(false)
,m_width(width)
,m_height(height)
,m_drawBuffers(NULL)
,m_colorBuffers(NULL)
{
   m_supported = true;

   if(!(glGenFramebuffersEXT && glDeleteFramebuffersEXT && glBindFramebufferEXT && glCheckFramebufferStatusEXT &&
      glGetFramebufferAttachmentParameterivEXT && glGenerateMipmapEXT && glFramebufferTexture2DEXT &&
      glFramebufferRenderbufferEXT && glGenRenderbuffersEXT && glDeleteRenderbuffersEXT &&
      glBindRenderbufferEXT && glRenderbufferStorageEXT && glGetRenderbufferParameterivEXT && glIsRenderbufferEXT))
   {
      m_supported = false;
      cout << "Video card does NOT support GL_EXT_framebuffer_object." << endl;
   }

   assert(m_supported);


   glGenFramebuffersEXT(1, &m_fboId);
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fboId);

   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
   glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &m_maxColorAttachments);

   m_colorBuffers = new GL_FBOBufferType[m_maxColorAttachments];
   m_drawBuffers = new GLenum[m_maxColorAttachments + 2];

   for (int i = 0; i < m_maxColorAttachments; ++i)
      m_colorBuffers[i].bufferTarget = GL_COLOR_ATTACHMENT0_EXT+i;

   m_dephtBuffer.bufferTarget = GL_DEPTH_ATTACHMENT_EXT;
   m_stencilBuffer.bufferTarget = GL_STENCIL_ATTACHMENT_EXT;

   m_numBuffers = 0;
}

GLFrameBufferObject :: ~GLFrameBufferObject()
{
  vector<GLRenderBufferObject*> :: iterator  rbIt;
  for(rbIt = m_renderBuffers.begin();rbIt!=m_renderBuffers.end();++rbIt)
    delete *rbIt;
  m_renderBuffers.clear();

  if(m_colorBuffers)
    delete[] m_colorBuffers;

  if(m_drawBuffers)
    delete[] m_drawBuffers;

  if(m_supported)
    glDeleteFramebuffersEXT(1, &m_fboId);
}


GLuint GLFrameBufferObject :: attachToColorBuffer(GL_FBOBufferType::BindType bt, int index, GLuint id)
{
   if(index == -1)
   {
     for (index = 0; index < m_maxColorAttachments; ++index)
         if(m_colorBuffers[index].bindType == GL_FBOBufferType::NotBind)
            break;

      if(index >= m_maxColorAttachments)
      {
         cout << "Max ColorAttachments Reached in FRAME BUFFER OBJECT" <<endl;
         assert(false);
      }
   }

   if(id == -1)
   {
      switch(bt)
      {
         case GL_FBOBufferType::Texture:
            id = createTextureToRender(m_width, m_height);
         break;
         case GL_FBOBufferType::RenderBufferObject:
            id = createRenderBufferObjectToRender(m_width, m_height);
         break;

         case GL_FBOBufferType::NotBind:
         default:
         break;
      }
   }

   m_colorBuffers[index].bindType = bt;
   m_colorBuffers[index].id = id;

   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fboId);
   m_colorBuffers[index].bind();
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

   //    check FBO status
   bool status = checkFramebufferStatus();
   if(!checkFramebufferStatus())
      m_supported = false;
   assert(m_supported);

  
  
  m_drawBuffers[m_numBuffers] = m_colorBuffers[index].bufferTarget;
  m_numBuffers = max(m_numBuffers, index + 1);

   return id;
}

GLuint GLFrameBufferObject :: attachToDepthBuffer(GL_FBOBufferType::BindType bt, GLuint id)
{
   if(id == -1)
   {
      switch(bt)
      {
         case GL_FBOBufferType::Texture:
            id = createTextureToRender(m_width, m_height);
         break;
         case GL_FBOBufferType::RenderBufferObject:
            id = createRenderBufferObjectToRender(m_width, m_height);
         break;

         case GL_FBOBufferType::NotBind:
         default:
         break;
      }
   }

   m_dephtBuffer.bindType = bt;
   m_dephtBuffer.id = id;

   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fboId);
   m_dephtBuffer.bind();
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

   //    check FBO status
   bool status = checkFramebufferStatus();
   if(!checkFramebufferStatus())
      m_supported = false;
   assert(m_supported);

   return id;
}

GLuint GLFrameBufferObject :: attachToStencilBuffer(GL_FBOBufferType::BindType bt, GLuint id)
{
   if(id == -1)
   {
      switch(bt)
      {
         case GL_FBOBufferType::Texture:
            id = createTextureToRender(m_width, m_height);
         break;
         case GL_FBOBufferType::RenderBufferObject:
            id = createRenderBufferObjectToRender(m_width, m_height);
         break;

         case GL_FBOBufferType::NotBind:
         default:
         break;
      }
   }

   m_stencilBuffer.bindType = bt;
   m_stencilBuffer.id = id;
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fboId);
   m_stencilBuffer.bind();
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

   //    check FBO status
   bool status = checkFramebufferStatus();
   if(!checkFramebufferStatus())
      m_supported = false;
   assert(m_supported);

   return id;
}

void GLFrameBufferObject::setActive(bool active)
{
   m_active = active;
   if(m_active)
   {
      glPushAttrib(GL_ALL_ATTRIB_BITS); 
      glDisable(GL_ALPHA_TEST);
      glDisable(GL_BLEND);

      glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fboId);

      glDrawBuffers(m_numBuffers, m_drawBuffers);
   }
   else
   {
     glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
     glPopAttrib();
   }
}

bool GLFrameBufferObject :: isActive()
{
   return m_active;
}

/************************************************************************/
/*  GLFrameBufferObject - PRIVATE                                       */
/************************************************************************/
GLuint GLFrameBufferObject::createTextureToRender(int width, int height)
{
   GLTextureObject t;
   t.createFBODataTexture(width, height);
   //glGenTextures(1, &textureId);
   //glBindTexture(GL_TEXTURE_2D, textureId);
   ////glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   ////glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   ////glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   ////glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   ////glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap generation included in OpenGL v1.4
   //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, width, height, 0, GL_RGBA, GL_FLOAT, 0);
   ////glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32UI_EXT, width, height, 0, GL_RGBA, GL_UNSIGNED_INT, 0);
   //glBindTexture(GL_TEXTURE_2D, 0);

   return t.getId();
}

GLuint GLFrameBufferObject::createRenderBufferObjectToRender(int width, int height)
{
   GLRenderBufferObject *r = new GLRenderBufferObject(width, height);
   m_renderBuffers.push_back(r);
   return r->getId();
}

bool GLFrameBufferObject::checkFramebufferStatus()
{
    // check FBO status
    GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    switch(status)
    {
    case GL_FRAMEBUFFER_COMPLETE_EXT:
        return true;

    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
        std::cout << "[ERROR] Framebuffer incomplete: Attachment is NOT complete." << std::endl;
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
        std::cout << "[ERROR] Framebuffer incomplete: No image is attached to FBO." << std::endl;
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
        std::cout << "[ERROR] Framebuffer incomplete: Attached images have different dimensions." << std::endl;
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
        std::cout << "[ERROR] Framebuffer incomplete: Color attached images have different internal formats." << std::endl;
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
        std::cout << "[ERROR] Framebuffer incomplete: Draw buffer." << std::endl;
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
        std::cout << "[ERROR] Framebuffer incomplete: Read buffer." << std::endl;
        return false;

    case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
        std::cout << "[ERROR] Unsupported by FBO implementation." << std::endl;
        return false;

    default:
        std::cout << "[ERROR] Unknow error." << std::endl;
        return false;
    }
}
