/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Mar 2010
 *
 *  OpenGL Camera Attributes Container
 */
#include <iostream>
#include <assert.h>

#include "ScScene/ScCamera.h"

using namespace std;

ScCamera :: ScCamera(Vector3 pos, Vector3 at, Vector3 up, float fovy,
         float near, float far, int screenWidth, int screenHeight)
:m_pos(pos)
,m_at(at)
,m_up(up)
,m_fovy(fovy)
,m_near(near)
,m_far(far)
{
}

void ScCamera :: readFromStr(char buffer[])
{
   int r = sscanf( buffer, "%f %f %f %f %f %f %f %f %f %f %f %f\n", &m_pos.x, &m_pos.y, &m_pos.z,
      &m_at.x, &m_at.y, &m_at.z, &m_up.x, &m_up.y, &m_up.z, &m_fovy, &m_near, &m_far);
   assert(r == 12);
}

Vector3 ScCamera::getPos() const
{
  return m_pos;
}

Vector3 ScCamera::getAt() const
{
  return m_at;
}

Vector3 ScCamera::getUp() const
{
  return m_up;
}

float ScCamera::getFovy() const
{
  return m_fovy;
}

float ScCamera::getNear() const
{
  return m_near;
}

float ScCamera::getFar() const
{
  return m_far;
}
