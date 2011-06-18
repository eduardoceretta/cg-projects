/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  May 2011
 *
 *  OpenGL Texture information extractor.
 */
#include <GL/glew.h>
#include <iostream>
#include "GLTextureObject.h"

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

