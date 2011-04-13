/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#include <GL/glew.h>
#include <iostream>
#include "GraphBasis/TextureObject.h"

using namespace std;

///////////////////
//~ TextureObject
//////////////////
TextureObject::TextureObject()
:m_buffer(NULL)
,m_id(0)
{

}

TextureObject::TextureObject(GLuint id)
:m_buffer(NULL)
,m_id(0)
{
  setId(id);
}

TextureObject :: ~TextureObject()
{
  delete[] m_buffer;
}

GLuint TextureObject :: getId()
{
   return m_id;
}

void TextureObject :: setId(GLuint id)
{
  if(id == m_id)
    return;

  if(m_buffer)
    delete[] m_buffer;
  m_id = id;
  m_buffer = new GLfloat[getTextureWidth()*getTextureHeight()*4];
}


GLuint TextureObject :: getTextureWidth()
{
  GLint textureWidth;
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, m_id);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &textureWidth);
  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_TEXTURE_2D);
  return textureWidth;
}

GLuint TextureObject :: getTextureHeight()
{
  GLint  textureHeight;
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, m_id);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &textureHeight);
  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_TEXTURE_2D);
  return textureHeight;
}

GLfloat* TextureObject :: getTextureData()
{
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, m_id);

  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, m_buffer);
  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_TEXTURE_2D);
  return m_buffer;
}

void TextureObject::renderTexture()
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

///////////////////
//~ Private
//////////////////