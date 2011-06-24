/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  May 2011
 *
 *  OpenGL Texture information extractor.
 *  Saves to a output image using the external lib FreeImage.
 */
#include <GL/glew.h>
#include <iostream>
#include "GLTextureObject.h"

#include "FreeImage/FreeImage.h"


using namespace std;

GLTextureObject::GLTextureObject()
:m_buffer(NULL)
,m_id(0)
{

}

GLTextureObject::GLTextureObject(GLuint id)
:m_buffer(NULL)
,m_id(0)
{
  setId(id);
}

GLTextureObject :: ~GLTextureObject()
{
  delete[] m_buffer;
}

GLuint GLTextureObject :: getId()
{
   return m_id;
}

void GLTextureObject :: setId(GLuint id)
{
  if(id == m_id)
    return;

  if(m_buffer)
    delete[] m_buffer;
  m_id = id;
  m_buffer = new GLfloat[getTextureWidth()*getTextureHeight()*4];
}


GLuint GLTextureObject :: getTextureWidth()
{
  GLint textureWidth;
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, m_id);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &textureWidth);
  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_TEXTURE_2D);
  return textureWidth;
}

GLuint GLTextureObject :: getTextureHeight()
{
  GLint  textureHeight;
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, m_id);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &textureHeight);
  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_TEXTURE_2D);
  return textureHeight;
}

GLfloat* GLTextureObject :: getTextureData()
{
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, m_id);

  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, m_buffer);
  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_TEXTURE_2D);
  return m_buffer;
}

void GLTextureObject::renderTexture()
{
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0, 1, 0, 1);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glEnable(GL_TEXTURE_2D);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_id);
  glColor3f(1,1,1);
  glBegin(GL_QUADS);
  glTexCoord2f(0,0); glVertex3f(0,0,0);
  glTexCoord2f(1,0); glVertex3f(1,0,0);
  glTexCoord2f(1,1); glVertex3f(1,1,0);
  glTexCoord2f(0,1); glVertex3f(0,1,0);
  glEnd();
  glBindTexture(GL_TEXTURE_2D, 0);
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}

void GLTextureObject::writeToFile(string fileName, ImageFileType fileType /*= PNG*/ )
{
  int w = getTextureWidth();
  int h = getTextureHeight();
  float *data = getTextureData();

  int bpp;
  FREE_IMAGE_FORMAT fif;
  switch(fileType)
  {
  case BMP:
    fif = FIF_BMP;
    bpp = 32;
    break;

  case JPG:
    fif = FIF_JPEG;
    bpp = 24;
    break;

  case PNG:
    fif = FIF_PNG;
    bpp = 32;
    break;

  default:
    fif = (FREE_IMAGE_FORMAT)fileType;
    bpp = 32;
    break;
  }


  // FreeImage_Initialise();
  FIBITMAP *dib = FreeImage_Allocate(w, h, bpp);

  unsigned width  = FreeImage_GetWidth(dib);
  unsigned height = FreeImage_GetHeight(dib);
  unsigned pitch  = FreeImage_GetPitch(dib);
  unsigned bitspp = FreeImage_GetBPP(dib);
  FREE_IMAGE_TYPE image_type = FreeImage_GetImageType(dib);

  if((image_type == FIT_BITMAP)) {
    BYTE *bits = (BYTE*)FreeImage_GetBits(dib);
    for(int y = 0; y < height; y++) {
      BYTE *pixel = (BYTE*)bits;
      for(int x = 0; x < width; x++) {

        pixel[FI_RGBA_RED]   = (BYTE) int(data[y*width*4 + x*4]*255.f);
        pixel[FI_RGBA_GREEN] = (BYTE) int(data[y*width*4 + x*4+1]*255.f);
        pixel[FI_RGBA_BLUE]  = (BYTE) int(data[y*width*4 + x*4+2]*255.f);
        if(bitspp == 32)
        {
          pixel[FI_RGBA_ALPHA] = (BYTE) int(255.f);
          pixel += 4;
        }else {
          pixel += 3;
        }
      }
      // next line
      bits += pitch;
    }
  }



  if (FreeImage_Save(fif, dib, fileName.c_str())) {
    // bitmap successfully saved!
  }
  //FreeImage_DeInitialise();

}
