/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/

#pragma once

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

#include "assert.h"
#include <string>
#include <vector>

typedef struct vbobuffer
{
   void* data;
   GLenum type;
   int n;
   int offset;
   GLenum clientState;

   vbobuffer()
      :data(NULL)
      ,type(sizeof(GL_BYTE))
      ,n(0)
      ,offset(0)
      ,clientState(GL_VERTEX_ARRAY)
   {}

   int sizeInBytes()
   {
      switch(clientState)
      {
         case GL_NORMAL_ARRAY:
         case GL_COLOR_ARRAY:
//         case GL_SECONDARY_COLOR_ARRAY:
         case GL_VERTEX_ARRAY:
            return n*sizeof(type)*3;
         case GL_TEXTURE_COORD_ARRAY:
            return n*sizeof(type)*2;
         case GL_INDEX_ARRAY:
//         case GL_FOG_COORD_ARRAY:
         case GL_EDGE_FLAG_ARRAY:
            return n*sizeof(type);
      }
   }

   void setPointer()
   {
      switch(clientState)
      {
         case GL_NORMAL_ARRAY:
            glNormalPointer(type, 0, (GLvoid*)offset);
         break;
         case GL_COLOR_ARRAY:
//         case GL_SECONDARY_COLOR_ARRAY:
            glColorPointer(3, type,  0, (GLvoid*)offset);
         break;
         case GL_VERTEX_ARRAY:
            glVertexPointer(3, type,  0, (GLvoid*)offset);
         break;
         case GL_TEXTURE_COORD_ARRAY:
            glTexCoordPointer(2, type,  0, (GLvoid*)offset);
         break;
         case GL_INDEX_ARRAY:
            glIndexPointer(type, 0, (GLvoid*)offset);
         break;
//         case GL_FOG_COORD_ARRAY:
//            glFogCoordPointer(type, 0, (GLvoid*)offset);
//         break;
         case GL_EDGE_FLAG_ARRAY:
            glEdgeFlagPointer(0, (GLvoid*)offset);
         break;
      }
   }
}VBOBuffer;


using namespace std;

class VertexBufferObject
{
   static bool sIsSupported();

   bool mSupported;
   bool mActive;

   GLuint mvboIndicesId;

   GLenum mPrimitive;

   int mVBOBuffersTotalSize;

   int  mModified;
   bool mCalculed;

   vector<VBOBuffer> mVBOBuffers;
   VBOBuffer mVBOIndexBuffer;

public:
   GLuint mvboId;
   VertexBufferObject(GLenum primitive = GL_TRIANGLES);
   ~VertexBufferObject();

   void clear();

   void calcVBO();

   void configure();
   void render();

   void setVBOBuffer(GLenum clientState, GLenum type, int n, void* data);
   void setVBOIndexBuffer(GLenum type, int n, void* data);
private:

};













//*Example

//   GLfloat vertices[] = {1,1,1,  -1,1,1,  -1,-1,1,  1,-1,1,        // v0-v1-v2-v3
//                      1,1,1,  1,-1,1,  1,-1,-1,  1,1,-1,        // v0-v3-v4-v5
//                      1,1,1,  1,1,-1,  -1,1,-1,  -1,1,1,        // v0-v5-v6-v1
//                      -1,1,1,  -1,1,-1,  -1,-1,-1,  -1,-1,1,    // v1-v6-v7-v2
//                      -1,-1,-1,  1,-1,-1,  1,-1,1,  -1,-1,1,    // v7-v4-v3-v2
//                      1,-1,-1,  -1,-1,-1,  -1,1,-1,  1,1,-1};   // v4-v7-v6-v5
//
//   GLfloat normals[] = {0,0,1,  0,0,1,  0,0,1,  0,0,1,             // v0-v1-v2-v3
//                     1,0,0,  1,0,0,  1,0,0, 1,0,0,              // v0-v3-v4-v5
//                     0,1,0,  0,1,0,  0,1,0, 0,1,0,              // v0-v5-v6-v1
//                     -1,0,0,  -1,0,0, -1,0,0,  -1,0,0,          // v1-v6-v7-v2
//                     0,-1,0,  0,-1,0,  0,-1,0,  0,-1,0,         // v7-v4-v3-v2
//                     0,0,-1,  0,0,-1,  0,0,-1,  0,0,-1};        // v4-v7-v6-v5
//
//   GLfloat colors[] = {1,1,1,  1,1,0,  1,0,0,  1,0,1,              // v0-v1-v2-v3
//                    1,1,1,  1,0,1,  0,0,1,  0,1,1,              // v0-v3-v4-v5
//                    1,1,1,  0,1,1,  0,1,0,  1,1,0,              // v0-v5-v6-v1
//                    1,1,0,  0,1,0,  0,0,0,  1,0,0,              // v1-v6-v7-v2
//                    0,0,0,  0,0,1,  1,0,1,  1,0,0,              // v7-v4-v3-v2
//                    0,0,1,  0,0,0,  0,1,0,  0,1,1};             // v4-v7-v6-v5
//
//
//   GLubyte indices[] = {0,1,2,3,
//                     4,5,6,7,
//                     8,9,10,11,
//                     12,13,14,15,
//                     16,17,18,19,
//                     20,21,22,23};
//
//
//   vbo = new VertexBufferObject(GL_QUADS);
//   vbo->setVBOBuffer( GL_VERTEX_ARRAY, GL_FLOAT, sizeof(vertices)/sizeof(GLfloat)/3, vertices); sizeof(vertices)/sizeof(GLfloat)/3 == 24
//   vbo->setVBOBuffer( GL_COLOR_ARRAY, GL_FLOAT, sizeof(colors)/sizeof(GLfloat)/3, colors);
//   vbo->setVBOBuffer( GL_COLOR_ARRAY, GL_FLOAT, sizeof(normals)/sizeof(GLfloat)/3, normals);
//   vbo->setVBOIndexBuffer( GL_UNSIGNED_BYTE, sizeof(indices)/sizeof(GLubyte), indices); //Not necessary
//   vbo->calcVBO();

//...

//vbo->render();
