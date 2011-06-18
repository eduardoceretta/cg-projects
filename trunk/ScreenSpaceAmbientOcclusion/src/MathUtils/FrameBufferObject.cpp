/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#include <GL/glew.h>
#include <iostream>
#include <assert.h>
#include "GraphBasis/FrameBufferObject.h"
#include "GraphBasis/RenderBufferObject.h"

using namespace std;

///////////////////
//~ FrameBufferObject
//////////////////

FrameBufferObject::FrameBufferObject()
:m_drawBuffers(NULL)
,m_collorBuffers(NULL)
{}


FrameBufferObject::FrameBufferObject(int width, int height)
:m_active(false)
,m_width(width)
,m_height(height)
,m_drawBuffers(NULL)
,m_collorBuffers(NULL)
{
	
   m_supported = true;
   /*GLenum err = glewInit();
   if (GLEW_OK != err)
   {
      mSupported = false;
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
   }

   if (!glewIsSupported("GL_VERSION_2_0"))
   {
      mSupported = false;
      printf("OpenGL 2.0 not supported\n");
   }*/

   if(!(glGenFramebuffersEXT && glDeleteFramebuffersEXT && glBindFramebufferEXT && glCheckFramebufferStatusEXT &&
      glGetFramebufferAttachmentParameterivEXT && glGenerateMipmapEXT && glFramebufferTexture2DEXT &&
      glFramebufferRenderbufferEXT && glGenRenderbuffersEXT && glDeleteRenderbuffersEXT &&
      glBindRenderbufferEXT && glRenderbufferStorageEXT && glGetRenderbufferParameterivEXT && glIsRenderbufferEXT))
   {
      m_supported = false;
      cout << "Video card does NOT support GL_EXT_framebuffer_object." << endl;
   }



   //GLuint rboId;
   //glGenRenderbuffersEXT(1, &rboId);
   //glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rboId);
   //glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT,
   //  mWidth, mHeight);
   //glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

   assert(m_supported);
   glGenFramebuffersEXT(1, &m_fboId);
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fboId);

   // attach the renderbuffer to depth attachment point
   /*glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
     GL_RENDERBUFFER_EXT, rboId);*/


   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
   glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &m_maxColorAttachments);

   m_collorBuffers = new BufferType[m_maxColorAttachments];
   m_drawBuffers = new GLenum[m_maxColorAttachments + 2];

   for (int i = 0; i < m_maxColorAttachments; ++i)
      m_collorBuffers[i].textureTarget = GL_COLOR_ATTACHMENT0_EXT+i;

   m_dephtBuffer.textureTarget = GL_DEPTH_ATTACHMENT_EXT;
   m_stencilBuffer.textureTarget = GL_STENCIL_ATTACHMENT_EXT;

   m_numBuffers = 0;
}

FrameBufferObject :: ~FrameBufferObject()
{
  vector<RenderBufferObject*> :: iterator  rbIt;
  for(rbIt = m_renderBuffers.begin();rbIt!=m_renderBuffers.end();++rbIt)
    delete *rbIt;
  m_renderBuffers.clear();

  if(m_collorBuffers)
    delete[] m_collorBuffers;

  if(m_drawBuffers)
    delete[] m_drawBuffers;

  if(m_supported)
    glDeleteFramebuffersEXT(1, &m_fboId);
}


GLuint FrameBufferObject :: attachToColorBuffer(BufferType::BindType bt, int index, GLuint id)
{
   if(index == -1)
   {
     for (index = 0; index < m_maxColorAttachments; ++index)
         if(m_collorBuffers[index].bindType == BufferType::NotBind)
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
         case BufferType::Texture:
            id = createTextureToRender(m_width, m_height);
         break;
         case BufferType::RenderBufferObject:
            id = createRenderFrameObjectToRender(m_width, m_height);
         break;

         case BufferType::NotBind:
         default:
         break;
      }
   }

   m_collorBuffers[index].bindType = bt;
   m_collorBuffers[index].id = id;

   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fboId);
   m_collorBuffers[index].bind();
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

   //    check FBO status
   bool status = checkFramebufferStatus();
   if(!checkFramebufferStatus())
      m_supported = false;
   assert(m_supported);

  
  
  m_drawBuffers[m_numBuffers] = m_collorBuffers[index].textureTarget;
  m_numBuffers = max(m_numBuffers, index + 1);
  //m_numBuffers++;

   return id;
}

GLuint FrameBufferObject :: attachToDepthBuffer(BufferType::BindType bt, GLuint id)
{
   if(id == -1)
   {
      switch(bt)
      {
         case BufferType::Texture:
            id = createTextureToRender(m_width, m_height);
         break;
         case BufferType::RenderBufferObject:
            id = createRenderFrameObjectToRender(m_width, m_height);
         break;

         case BufferType::NotBind:
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

   //m_drawBuffers[m_numBuffers++] = mDephtBuffer.textureTarget;

   return id;
}

GLuint FrameBufferObject :: attachToStencilBuffer(BufferType::BindType bt, GLuint id)
{
   if(id == -1)
   {
      switch(bt)
      {
         case BufferType::Texture:
            id = createTextureToRender(m_width, m_height);
         break;
         case BufferType::RenderBufferObject:
            //glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mfboId);
            id = createRenderFrameObjectToRender(m_width, m_height);
            //glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
         break;

         case BufferType::NotBind:
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

   //m_drawBuffers[m_numBuffers++] = mStencilBuffer.textureTarget;

   return id;
}

void FrameBufferObject::setActive(bool active)
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

bool FrameBufferObject :: isActive()
{
   return m_active;
}


///////////////////
//~ Private
//////////////////
GLuint FrameBufferObject::createTextureToRender(int width, int height)
{
   GLuint textureId;
   glGenTextures(1, &textureId);
   glBindTexture(GL_TEXTURE_2D, textureId);
//   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap generation included in OpenGL v1.4
   //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, width, height, 0, GL_RGBA, GL_FLOAT, 0);
   glBindTexture(GL_TEXTURE_2D, 0);

   return textureId;
}

GLuint FrameBufferObject::createRenderFrameObjectToRender(int width, int height)
{
   RenderBufferObject *r = new RenderBufferObject(width, height);
   m_renderBuffers.push_back(r);
   return r->getId();
}

bool FrameBufferObject::checkFramebufferStatus()
{
    // check FBO status
    GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    switch(status)
    {
    case GL_FRAMEBUFFER_COMPLETE_EXT:
//        std::cout << "Framebuffer complete." << std::endl;
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
