/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/

#ifndef FrameBufferObject_H
#define FrameBufferObject_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <vector>

#include "GraphBasis\RenderBufferObject.h"
using namespace std;

typedef struct bufferType{
   enum BindType {Texture, RenderBufferObject, NotBind};
   BindType bindType;
   GLuint id;
   GLenum textureTarget;

   bufferType()
      :bindType(NotBind)
      ,textureTarget(GL_COLOR_ATTACHMENT0_EXT)
      {}

   void bind()
   {
      switch(bindType)
      {
         case Texture:
            glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, textureTarget, GL_TEXTURE_2D, id, 0);
         break;
         case RenderBufferObject:
            glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, textureTarget, GL_RENDERBUFFER_EXT, id);
         break;

         case NotBind:
         default:
         break;
      }
   }
}BufferType;

class FrameBufferObject
{
   bool m_supported;
   bool m_active;
   GLuint m_fboId;

   int m_width;
   int m_height;
   int m_numBuffers;
    
   GLint m_maxColorAttachments;

   BufferType *m_collorBuffers;// [GL_MAX_COLOR_ATTACHMENTS_EXT];
   BufferType m_dephtBuffer;
   BufferType m_stencilBuffer;

   vector<RenderBufferObject*> m_renderBuffers;
public:
   FrameBufferObject();
   FrameBufferObject(int width, int height);
   ~FrameBufferObject();

   GLuint attachToColorBuffer(BufferType::BindType bt, int index = -1, GLuint id = -1);
   GLuint attachToDepthBuffer(BufferType::BindType bt, GLuint id = -1);
   GLuint attachToStencilBuffer(BufferType::BindType bt, GLuint id = -1);

   void setActive(bool);
   bool isActive();

   GLenum *m_drawBuffers; //[GL_MAX_COLOR_ATTACHMENTS_EXT+2];

private:
   GLuint createTextureToRender(int width, int height);
   GLuint createRenderFrameObjectToRender(int width, int height);
   bool checkFramebufferStatus();
};

#endif


