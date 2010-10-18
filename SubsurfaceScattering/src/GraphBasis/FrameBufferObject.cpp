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

FrameBufferObject::FrameBufferObject(){}


FrameBufferObject::FrameBufferObject(int width, int height)
:mActive(false)
,mWidth(width)
,mHeight(height)
{
   mSupported = true;
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
      mSupported = false;
      cout << "Video card does NOT support GL_EXT_framebuffer_object." << endl;
   }

   int max_size; 
   glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE_EXT, &max_size);
   if(width > max_size || height > max_size)
   {
     mSupported = false;
     cout << "Video card does NOT support sizes bigger than "<< max_size << "." << endl;
   }



   //GLuint rboId;
   //glGenRenderbuffersEXT(1, &rboId);
   //glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rboId);
   //glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT,
   //  mWidth, mHeight);
   //glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

   assert(mSupported);
   glGenFramebuffersEXT(1, &mfboId);
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mfboId);

   // attach the renderbuffer to depth attachment point
   /*glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
     GL_RENDERBUFFER_EXT, rboId);*/


   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

   for (int i = 0; i < GL_MAX_COLOR_ATTACHMENTS_EXT; ++i)
      mCollorBuffers[i].textureTarget = GL_COLOR_ATTACHMENT0_EXT+i;

   mDephtBuffer.textureTarget = GL_DEPTH_ATTACHMENT_EXT;
   mStencilBuffer.textureTarget = GL_STENCIL_ATTACHMENT_EXT;

   m_numBuffers = 0;
}

FrameBufferObject :: ~FrameBufferObject()
{
  vector<RenderBufferObject*> :: iterator  rbIt;
  for(rbIt = mRenderBuffers.begin();rbIt!=mRenderBuffers.end();++rbIt)
    delete *rbIt;
  mRenderBuffers.clear();
  if(mSupported)
    glDeleteFramebuffersEXT(1, &mfboId);
}


GLuint FrameBufferObject :: attachToColorBuffer(BufferType::BindType bt, int index, GLuint id)
{
   if(index == -1)
   {
      for (index = 0; index < GL_MAX_COLOR_ATTACHMENTS_EXT; ++index)
         if(mCollorBuffers[index].bindType == BufferType::NotBind)
            break;

      if(index >= GL_MAX_COLOR_ATTACHMENTS_EXT)
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
            id = createTextureToRender(mWidth, mHeight);
         break;
         case BufferType::RenderBufferObject:
            id = createRenderFrameObjectToRender(mWidth, mHeight);
         break;

         case BufferType::NotBind:
         default:
         break;
      }
   }

   mCollorBuffers[index].bindType = bt;
   mCollorBuffers[index].id = id;

   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mfboId);
   mCollorBuffers[index].bind();
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

   //    check FBO status
   bool status = checkFramebufferStatus();
   if(!checkFramebufferStatus())
      mSupported = false;
   assert(mSupported);


  m_drawBuffers[m_numBuffers++] = mCollorBuffers[index].textureTarget;

   return id;
}

GLuint FrameBufferObject :: attachToDepthBuffer(BufferType::BindType bt, GLuint id)
{
   if(id == -1)
   {
      switch(bt)
      {
         case BufferType::Texture:
            id = createTextureToRender(mWidth, mHeight);
         break;
         case BufferType::RenderBufferObject:
            id = createRenderFrameObjectToRender(mWidth, mHeight);
         break;

         case BufferType::NotBind:
         default:
         break;
      }
   }

   mDephtBuffer.bindType = bt;
   mDephtBuffer.id = id;

   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mfboId);
   mDephtBuffer.bind();
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

   //    check FBO status
   bool status = checkFramebufferStatus();
   if(!checkFramebufferStatus())
      mSupported = false;
   assert(mSupported);

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
            id = createTextureToRender(mWidth, mHeight);
         break;
         case BufferType::RenderBufferObject:
            //glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mfboId);
            id = createRenderFrameObjectToRender(mWidth, mHeight);
            //glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
         break;

         case BufferType::NotBind:
         default:
         break;
      }
   }

   mStencilBuffer.bindType = bt;
   mStencilBuffer.id = id;
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mfboId);
   mStencilBuffer.bind();
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

   //    check FBO status
   bool status = checkFramebufferStatus();
   if(!checkFramebufferStatus())
      mSupported = false;
   assert(mSupported);

   //m_drawBuffers[m_numBuffers++] = mStencilBuffer.textureTarget;

   return id;
}

void FrameBufferObject::setActive(bool active)
{
   mActive = active;
   if(mActive)
   {
      glPushAttrib(GL_ALL_ATTRIB_BITS); 
      glDisable(GL_ALPHA_TEST);
      glDisable(GL_BLEND);

      glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mfboId);

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
   return mActive;
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
   mRenderBuffers.push_back(r);
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
