/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/

#ifndef RenderBufferObject_H
#define RenderBufferObject_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>

#include "assert.h"
#include <string>
using namespace std;

class RenderBufferObject
{
   bool mSupported;
   GLuint mrboId;

   int mWidth;
   int mHeight;
public:
   RenderBufferObject(int width, int height);
   ~RenderBufferObject();

   GLuint getId();
private:
};

#endif


