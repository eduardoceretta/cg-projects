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
,m_screenWidth(screenWidth)
,m_screenHeight(screenHeight)
{
}

void ScCamera :: readFromStr(char buffer[])
{
   int r = sscanf( buffer, "%f %f %f %f %f %f %f %f %f %f %f %f %d %d\n", &m_pos.x, &m_pos.y, &m_pos.z,
      &m_at.x, &m_at.y, &m_at.z, &m_up.x, &m_up.y, &m_up.z, &m_fovy, &m_near, &m_far, &m_screenWidth, &m_screenHeight);
   assert(r == 14);
}

