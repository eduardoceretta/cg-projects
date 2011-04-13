/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/

#ifndef _TEXTURE_OBJECT_H_
#define _TEXTURE_OBJECT_H_

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>

#include "assert.h"
#include <string>
using namespace std;

class TextureObject
{
   GLfloat *m_buffer;
   GLuint m_id;

 public:
   TextureObject();
   TextureObject(GLuint id);

   ~TextureObject();

   void setId(GLuint id);
   GLuint getId();

   GLfloat* getTextureData();
   GLuint getTextureHeight();
   GLuint getTextureWidth();

   void renderTexture();
  
private:
};

#endif


