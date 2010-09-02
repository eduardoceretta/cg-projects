/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#include <GL/glew.h>
#include <iostream>
#include "GraphBasis/VertexBufferObject.h"

using namespace std;


bool VertexBufferObject :: sIsSupported()
{
   static int supported = -1;
   if(supported != -1)
      return supported == 1;

   supported = 1;

   GLenum err = glewInit();
   if (GLEW_OK != err)
   {
      supported = 0;
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
   }

   if (!glewIsSupported("GL_VERSION_2_0"))
   {
      supported = 0;
      printf("OpenGL 2.0 not supported\n");
   }

   if(!glGenBuffersARB || !glBindBufferARB || !glBufferDataARB || !glBufferSubDataARB ||
         !glMapBufferARB || !glUnmapBufferARB || !glDeleteBuffersARB || !glGetBufferParameterivARB)
   {
      supported = 0;
      cout << "Video card does NOT support GL_ARB_vertex_buffer_object." << endl;
   }

   return supported == 1;
}

///////////////////
//~ VertexBufferObject
//////////////////
VertexBufferObject::VertexBufferObject(GLenum primitive)
:mPrimitive(primitive)
,mCalculated(false)
,mHasIndices(false)
,mvboId(0)
,mvboIndicesId(0)
,mVBOBuffersTotalSize(0)
{
   mSupported = sIsSupported();

   assert(mSupported);
   // printf("VBO suportado\n");
}

VertexBufferObject :: ~VertexBufferObject()
{
   if(mSupported && mvboId)
   {
      glDeleteBuffersARB(1, &mvboId);
      if(mvboIndicesId)
         glDeleteBuffersARB(1, &mvboIndicesId);
   }
}

void VertexBufferObject :: clear()
{
   mVBOBuffers.clear();
   mCalculated = false;
  
   glDeleteBuffersARB(1, &mvboId);
   if(mHasIndices == true)
   {
      glDeleteBuffersARB(1, &mvboIndicesId);
   }
}


void VertexBufferObject :: calcVBO()
{
   if(mCalculated==true)
      return;
  
   vector<VBOBuffer> :: iterator it;
   mVBOBuffersTotalSize = 0;
   for(it = mVBOBuffers.begin(); it != mVBOBuffers.end(); ++it)
   {
      it->offset = mVBOBuffersTotalSize;
      mVBOBuffersTotalSize += it->sizeInBytes();
   }

   glGenBuffersARB(1, &mvboId);
   glBindBufferARB(GL_ARRAY_BUFFER_ARB, mvboId);
   glBufferDataARB(GL_ARRAY_BUFFER_ARB, mVBOBuffersTotalSize, 0, GL_STATIC_DRAW_ARB); // Alocates space for the memory block

   for(it = mVBOBuffers.begin(); it != mVBOBuffers.end(); ++it)
      glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, it->offset, it->sizeInBytes(), it->data); //substitui os dados do buffer data
      
   if(mHasIndices == true)
   {
      glGenBuffersARB(1, &mvboIndicesId);
      glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, mvboIndicesId);
      glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, mVBOIndexBuffer.sizeInBytes(), mVBOIndexBuffer.data, GL_STATIC_DRAW_ARB);
   }

   mCalculated = true;
}

void VertexBufferObject :: configure()
{
}

void VertexBufferObject :: render()
{
   glBindBufferARB(GL_ARRAY_BUFFER_ARB, mvboId);
   if(mvboIndicesId)
      glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, mvboIndicesId);

   vector<VBOBuffer> :: iterator it;
   for(it = mVBOBuffers.begin(); it != mVBOBuffers.end(); ++it)
   {
      glEnableClientState(it->clientState);
      it->setPointer();
   }

   if(mHasIndices == true)
      glDrawElements(mPrimitive, mVBOIndexBuffer.n, mVBOIndexBuffer.type, 0);
   else
      glDrawArrays(mPrimitive, 0, mVBOBuffers.begin()->n);

   for(it = mVBOBuffers.begin(); it != mVBOBuffers.end(); ++it)
      glDisableClientState(it->clientState);

   if(mHasIndices == true)
      glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
   glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}


void VertexBufferObject :: setVBOBuffer(GLenum clientState, GLenum type, int n, void* data)
{
   mCalculated = false;
   
   if(clientState == GL_INDEX_ARRAY)
   {
      setVBOIndexBuffer(type, n, data);
      return;
   }

   VBOBuffer buff;
   buff.clientState = clientState;
   buff.type = type;
   buff.n = n;
   buff.data = data;

   mVBOBuffers.push_back(buff);
}

void VertexBufferObject :: setVBOIndexBuffer(GLenum type, int n, void* data)
{
   mHasIndices = true;
   mVBOIndexBuffer.clientState = GL_INDEX_ARRAY;
   mVBOIndexBuffer.type = type;
   mVBOIndexBuffer.n = n;
   mVBOIndexBuffer.data = data;
}




















