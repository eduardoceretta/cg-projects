/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
            
            adaptacao: cesar Tadeu Pozzer
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
   void* data; //vetor de dados

   GLenum type;
   int n;      ///numero total de Elementos do VBO (vertices, cores, normais, indices, etc)
   int offset; //calculado na funcao calcVBO()
   GLenum clientState; //GL_NORMAL_ARRAY, GL_COLOR_ARRAY, GL_SECONDARY_COLOR_ARRAY, GL_VERTEX_ARRAY, GL_TEXTURE_COORD_ARRAY, GL_INDEX_ARRAY:




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
         case GL_COLOR_ARRAY:  //assume cores em RGB e vertices em 3D

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

   //chamada na funcao render() do VBO
   void setPointer()
   {
      switch(clientState)
      {
         case GL_NORMAL_ARRAY:
            glNormalPointer(type, 0, (GLvoid*)offset);
         break;
         case GL_COLOR_ARRAY: //assume cores em RGB

            glColorPointer(3, type,  0, (GLvoid*)offset);
         break;
         case GL_VERTEX_ARRAY: //assume que sejam vertices em 3D

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
   bool mHasIndices;  //indica se esta sendo usado o vetor de indices


   GLuint mvboId;        //ID do ARRAY_BUFFER
   GLuint mvboIndicesId; //ID do ELEMENT_ARRAY_BUFFER - depende de mHasIndices


   GLenum mPrimitive;

   int mVBOBuffersTotalSize;

   bool mCalculated;



   vector<VBOBuffer> mVBOBuffers;
   VBOBuffer mVBOIndexBuffer;

public:


   VertexBufferObject(GLenum primitive = GL_TRIANGLES);
   ~VertexBufferObject();

   void clear();

   void calcVBO();

   void configure();
   void render();

   void setVBOBuffer(GLenum clientState, GLenum type, int n, void* data);
   void setVBOIndexBuffer(GLenum type, int n, void* data);


};
