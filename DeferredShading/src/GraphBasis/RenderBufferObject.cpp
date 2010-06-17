/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#include <GL/glew.h>
#include <iostream>
#include "GraphBasis/RenderBufferObject.h"

using namespace std;

///////////////////
//~ RenderBufferObject
//////////////////
RenderBufferObject::RenderBufferObject(int width, int height)
:mWidth(width)
,mHeight(height)
{
   mSupported = true;
   GLenum err = glewInit();
   if (GLEW_OK != err)
   {
      mSupported = false;
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
   }

   if (!glewIsSupported("GL_VERSION_2_0"))
   {
      mSupported = false;
      printf("OpenGL 2.0 not supported\n");
   }

   if(!(glGenFramebuffersEXT && glDeleteFramebuffersEXT && glBindFramebufferEXT && glCheckFramebufferStatusEXT &&
      glGetFramebufferAttachmentParameterivEXT && glGenerateMipmapEXT && glFramebufferTexture2DEXT &&
      glFramebufferRenderbufferEXT && glGenRenderbuffersEXT && glDeleteRenderbuffersEXT &&
      glBindRenderbufferEXT && glRenderbufferStorageEXT && glGetRenderbufferParameterivEXT && glIsRenderbufferEXT))
   {
      mSupported = false;
      cout << "Video card does NOT support GL_EXT_framebuffer_object." << endl;
   }

   assert(mSupported);

   glGenRenderbuffersEXT(1, &mrboId);
   glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, mrboId);
   glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, mWidth, mHeight);
   glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
}

RenderBufferObject :: ~RenderBufferObject()
{
   if(mSupported)
      glDeleteRenderbuffersEXT(1, &mrboId);
}

GLuint RenderBufferObject :: getId()
{
   return mrboId;
}

///////////////////
//~ Private
//////////////////
