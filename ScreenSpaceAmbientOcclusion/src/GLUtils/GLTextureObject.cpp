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
:m_fbuffer(NULL)
,m_uibuffer(NULL)
,m_id(0)
,m_target(GL_TEXTURE_2D)
{

}

GLTextureObject::GLTextureObject(GLuint id, GLenum target)
:m_fbuffer(NULL)
,m_uibuffer(NULL)
,m_id(0)
,m_target(target)
{
  setId(id, m_target);
}

GLTextureObject :: ~GLTextureObject()
{
  if(m_fbuffer)
    delete[] m_fbuffer;
  if(m_uibuffer)
    delete[] m_uibuffer;
}

GLuint GLTextureObject :: getId()
{
  return m_id;
}

void GLTextureObject :: setId(GLuint id, GLenum target)
{
  if(id == m_id)
    return;

  if(m_fbuffer)
    delete[] m_fbuffer;
  m_fbuffer = NULL;

  if(m_uibuffer)
    delete[] m_uibuffer;
  m_uibuffer = NULL;

  m_id = id;
  m_target = target;
}


GLuint GLTextureObject :: readTextureWidth()
{
  GLint textureWidth;
  glPushAttrib(GL_ENABLE_BIT|GL_TEXTURE_BIT);
  glEnable(m_target);
  glBindTexture(m_target, m_id);
  glGetTexLevelParameteriv(m_target, 0, GL_TEXTURE_WIDTH, &textureWidth);
  glPopAttrib();
  return textureWidth;
}

GLuint GLTextureObject :: readTextureHeight()
{
  GLint  textureHeight;
  glPushAttrib(GL_ENABLE_BIT|GL_TEXTURE_BIT);
  glEnable(m_target);
  glBindTexture(m_target, m_id);
  glGetTexLevelParameteriv(m_target, 0, GL_TEXTURE_HEIGHT, &textureHeight);
  glPopAttrib();
  return textureHeight;
}

GLuint GLTextureObject::readTextureNumColors()
{
  GLint rs, gs, bs, as, ls, is;
  glPushAttrib(GL_ENABLE_BIT|GL_TEXTURE_BIT);
  glEnable(m_target);
  glBindTexture(m_target, m_id);
  glGetTexLevelParameteriv(m_target, 0, GL_TEXTURE_RED_SIZE, &rs);
  glGetTexLevelParameteriv(m_target, 0, GL_TEXTURE_GREEN_SIZE, &gs);
  glGetTexLevelParameteriv(m_target, 0, GL_TEXTURE_BLUE_SIZE, &bs);
  glGetTexLevelParameteriv(m_target, 0, GL_TEXTURE_LUMINANCE_SIZE, &ls);
  glGetTexLevelParameteriv(m_target, 0, GL_TEXTURE_ALPHA_SIZE, &as);
  glPopAttrib();
  return int(rs!=0) + int(gs!=0) + int(bs!=0) + int(as!=0) + int(ls!=0);
}
GLfloat* GLTextureObject::read1DTextureFloatData(GLenum format /*= GL_RGBA*/)
{
  m_target = GL_TEXTURE_1D;
  if(!m_fbuffer)
    m_fbuffer = new GLfloat[readTextureWidth()*readTextureNumColors()];
  glPushAttrib(GL_ENABLE_BIT|GL_TEXTURE_BIT);
  glEnable(GL_TEXTURE_1D);
  glBindTexture(GL_TEXTURE_1D, m_id);

  glGetTexImage(GL_TEXTURE_1D, 0, format, GL_FLOAT, m_fbuffer);
  glBindTexture(GL_TEXTURE_1D, 0);
  glPopAttrib();
  return m_fbuffer;
}
GLfloat* GLTextureObject :: read2DTextureFloatData(GLenum format /*= GL_RGBA*/)
{
  m_target = GL_TEXTURE_2D;
  if(!m_fbuffer)
    m_fbuffer = new GLfloat[readTextureWidth()*readTextureHeight()*readTextureNumColors()];
  glPushAttrib(GL_ENABLE_BIT|GL_TEXTURE_BIT);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, m_id);

  glGetTexImage(GL_TEXTURE_2D, 0, format, GL_FLOAT, m_fbuffer);
  glBindTexture(GL_TEXTURE_2D, 0);
  glPopAttrib();
  return m_fbuffer;
}


GLuint* GLTextureObject::read1DTextureUIntData(GLenum format /*= GL_RGBA_INTEGER_EXT*/)
{
  m_target = GL_TEXTURE_1D;
  if(!m_uibuffer)
    m_uibuffer = new GLuint[readTextureWidth()*readTextureNumColors()];
  glPushAttrib(GL_ENABLE_BIT|GL_TEXTURE_BIT);
  glEnable(GL_TEXTURE_1D);
  glBindTexture(GL_TEXTURE_1D, m_id);

  glGetTexImage(GL_TEXTURE_1D, 0, format, GL_UNSIGNED_INT, m_uibuffer);
  glBindTexture(GL_TEXTURE_1D, 0);
  glPopAttrib();
  return m_uibuffer;
}


GLuint* GLTextureObject :: read2DTextureUIntData(GLenum format /*= GL_RGBA_INTEGER_EXT*/)
{
  m_target = GL_TEXTURE_2D;
  if(!m_uibuffer)
    m_uibuffer = new GLuint[readTextureWidth()*readTextureHeight()*readTextureNumColors()];
  glPushAttrib(GL_ENABLE_BIT|GL_TEXTURE_BIT);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, m_id);

  glGetTexImage(GL_TEXTURE_2D, 0, format, GL_UNSIGNED_INT, m_uibuffer);
  glBindTexture(GL_TEXTURE_2D, 0);
  glPopAttrib();
  return m_uibuffer;
}


void GLTextureObject::render2DTexture()
{
  glPushAttrib(GL_ENABLE_BIT|GL_TEXTURE_BIT|GL_CURRENT_BIT);
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
  glPopAttrib();
}

void GLTextureObject::write2DToFile(string path, string fileName, ImageFileType fileType /*= PNG*/ )
{
  int w = readTextureWidth();
  int h = readTextureHeight();
  float *data = read2DTextureFloatData();

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
  string pathFileName = path + string("/") + fileName;
  if (FreeImage_Save(fif, dib, pathFileName.c_str())) {
    // bitmap successfully saved!
  }
  //FreeImage_DeInitialise();

}

GLuint GLTextureObject::createTexture1D(int width, GLenum internalFormat, GLenum format, GLenum type, const GLvoid * data /*= 0*/ )
{
  GLuint textureId;
  glGenTextures(1, &textureId);
  setTexture1D(textureId, width, internalFormat, format, type, data);
  return textureId;
}

void GLTextureObject::setTexture1D(GLuint textureId, int width, GLenum internalFormat, GLenum format, GLenum type, const GLvoid * data /*= 0*/ )
{
  glPushAttrib(GL_ENABLE_BIT|GL_TEXTURE_BIT);
  setId(textureId, GL_TEXTURE_1D);
  glEnable(GL_TEXTURE_1D);
  glBindTexture(GL_TEXTURE_1D, m_id);

  glTexParameteri(GL_TEXTURE_1D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap generation included in OpenGL v1.4
  glTexImage1D(GL_TEXTURE_1D, 0, internalFormat, width, 0, format, type, data);
  glPopAttrib();
}

GLuint GLTextureObject::createTexture2D( int width, int height, GLenum internalFormat, GLenum format, GLenum type, const GLvoid * data /*= 0*/ )
{
  GLuint textureId;
  glGenTextures(1, &textureId);
  setTexture2D(textureId, width, height, internalFormat, format, type, data);
  return textureId;
}

void GLTextureObject::setTexture2D(GLuint textureId, int width, int height, GLenum internalFormat, GLenum format, GLenum type, const GLvoid * data /*= 0*/ )
{
  glPushAttrib(GL_ENABLE_BIT|GL_TEXTURE_BIT);
  setId(textureId, GL_TEXTURE_2D);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, m_id);

  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap generation included in OpenGL v1.4
  glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
  glPopAttrib();
}

void GLTextureObject::setFilters( GLfloat minFilter, GLfloat magFilter)
{
  glPushAttrib(GL_ENABLE_BIT|GL_TEXTURE_BIT);
  glEnable(m_target);
  glBindTexture(m_target, m_id);

  glTexParameterf(m_target, GL_TEXTURE_MAG_FILTER, magFilter);
  glTexParameterf(m_target, GL_TEXTURE_MIN_FILTER, minFilter);
  glPopAttrib();
}

void GLTextureObject::setWraps( GLfloat sWrap, GLfloat tWrap)
{
  glPushAttrib(GL_ENABLE_BIT|GL_TEXTURE_BIT);
  glEnable(m_target);
  glBindTexture(m_target, m_id);

  glTexParameterf(m_target, GL_TEXTURE_WRAP_S, sWrap);
  if(m_target == GL_TEXTURE_2D)
    glTexParameterf(m_target, GL_TEXTURE_WRAP_T, tWrap);
  glPopAttrib();
}

void GLTextureObject::createFBODataTexture(int width, int height)
{
  createTexture2D(width, height, GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT);
  setFilters(GL_NEAREST, GL_NEAREST);
  setWraps(GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE);
}




