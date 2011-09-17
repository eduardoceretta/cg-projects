/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Aug 2011
 *
 *  OpenGL Projection Matrix extractor.
 *  Gets the Projection [Perspective|Orthographic] Matrix and its information
 */
#include <GL/glew.h>
#include <iostream>
#include "GLProjectionMatrix.h"


using namespace std;


GLProjectionMatrix::GLProjectionMatrix()
:Matrix4()
,m_outdated(true)
,m_top(-1.0f)
,m_bottom(-1.0f)
,m_right(-1.0f)
,m_left(-1.0f)
,m_near(-1.0f)
,m_far(-1.0f)
,m_projType(GLProjectionMatrix::None)
{

}

GLProjectionMatrix::GLProjectionMatrix( float *m )
:Matrix4(m)
,m_outdated(true)
,m_top(-1.0f)
,m_bottom(-1.0f)
,m_right(-1.0f)
,m_left(-1.0f)
,m_near(-1.0f)
,m_far(-1.0f)
,m_projType(GLProjectionMatrix::None)
{

}

GLProjectionMatrix::~GLProjectionMatrix()
{

}

void GLProjectionMatrix::readGLProjection()
{
  GLfloat p[16];
  glGetFloatv(GL_PROJECTION_MATRIX, p);
  for(int i=0; i<4; ++i)
    for(int j = 0; j < 4; ++j)
      m_mat[i][j] = (double)p[i*4+j];
  m_outdated = true;
}
float GLProjectionMatrix::getNear() 
{
  update(); 
  return m_near;
}

float GLProjectionMatrix::getFar() 
{
  update(); 
  return m_far;
}

float GLProjectionMatrix::getLeft()
{
  update(); 
  return m_left;
}

float GLProjectionMatrix::getRight()
{
  update(); 
  return m_right;
}

float GLProjectionMatrix::getBottom()
{
  update();
  return m_bottom;
}

float GLProjectionMatrix::getTop()
{
  update(); 
  return m_top;
}

bool GLProjectionMatrix::isPerspective()
{
  update(); 
  return m_projType == GLProjectionMatrix::Perspective;
}

bool GLProjectionMatrix::isOrthographic()
{
  update(); 
  return m_projType == GLProjectionMatrix::Orthographic;
}

void GLProjectionMatrix::update()
{
  if(!m_outdated)
    return;

  if(m_mat[3][3] <= 0.000001f)
    m_projType = GLProjectionMatrix::Perspective;

  if(m_mat[3][3] >= .9999999f)
    m_projType = GLProjectionMatrix::Orthographic;

  switch(m_projType)
  {
    case GLProjectionMatrix::Perspective:
      updatePerspective();
      break;
    case GLProjectionMatrix::Orthographic:
      updateOrthographic();
      break;
    case GLProjectionMatrix::None:
    default:
      break;
  }

  m_outdated = false;
}

void GLProjectionMatrix::updatePerspective()
{
  float x = m_mat[0][0];
  float y = m_mat[1][1];
  float z = m_mat[2][2];
  float w = m_mat[3][2];
  float a = m_mat[2][0];
  float b = m_mat[2][1];

  m_near = w/(z - 1.0);
  m_far = w * m_near/(w + 2.0 * m_near);
  
  m_left = (1.0f - a)*2.0f*m_near/(-2.0f*x);
  m_right = (-a*m_left - m_left)/(1.0f-a);

  m_bottom = (1.0f - b)*2.0f*m_near/(-2.0f*y);
  m_top = (-b*m_bottom - m_bottom)/(1.0f-b);
}

void GLProjectionMatrix::updateOrthographic()
{
  float x = m_mat[0][0];
  float y = m_mat[1][1];
  float z = m_mat[2][2];
  float w = m_mat[3][2];
  float a = m_mat[3][0];
  float b = m_mat[3][1];

  m_near = (w + 1.0f)/z;
  m_far = -2.0f/z + m_near;

  m_left = (-a*x - 2.0f)/(2.0f*x);
  m_right = (2.0f)/x + m_left;

  m_bottom = (-b*y - 2.0f)/(2.0f*y);
  m_top = (2.0f)/y + m_bottom;
}
