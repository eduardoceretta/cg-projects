/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Nov 2011
 *
 *  Camera Handler. Simple OpenGl LookAt Camera
 */
#include <iostream>
#include <assert.h>
#include <cmath>

#include "Handlers/GLCameraHandler.h"
#include "GLLights/GLPointLight.h"

using namespace std;

GLCameraHandler::GLCameraHandler()
:m_pos(Vector3(0,0,0))
,m_at(Vector3(0,0,0))
,m_up(Vector3(0,0,0))
,m_lastMousePosX(0.0f)
,m_lastMousePosY(0.0f)
,m_mouseState(GLUT_UP)
,m_mouseButton(GLUT_RIGHT_BUTTON)
,m_modifier(0)
,m_minerLight(new GLPointLight())
,m_minerOn(false)
{
  m_minerLight->setRenderSphereEnabled(false);
}

GLCameraHandler::GLCameraHandler(Vector3 pos, Vector3 at, Vector3 up)
:m_pos(pos)
,m_at(at)
,m_up(up)
,m_lastMousePosX(0.0f)
,m_lastMousePosY(0.0f)
,m_mouseState(GLUT_UP)
,m_mouseButton(GLUT_RIGHT_BUTTON)
,m_modifier(0)
,m_minerLight(new GLPointLight())
,m_minerOn(false)
{
  m_minerLight->setRenderSphereEnabled(false);
}


GLCameraHandler::~GLCameraHandler()
{
  delete m_minerLight;
}


void GLCameraHandler::listenSpecialKeyboard(int key)
{
  m_modifier = glutGetModifiers();
}

void GLCameraHandler::listenMouseMove(float x, float y)
{
  m_lastMousePosX = x;
  m_lastMousePosY = y;
}

void GLCameraHandler::listenMouseClick(int button, int state, float x, float y)
{
  m_mouseState = state;
  m_mouseButton = button;

  m_lastMousePosX = x;
  m_lastMousePosY = y;
}

void GLCameraHandler::render()
{
  glLoadIdentity();
  gluLookAt(m_pos.x, m_pos.y , m_pos.z, 
            m_at.x, m_at.y, m_at.z,
            m_up.x, m_up.y, m_up.z);

  m_minerLight->setPosition(m_pos);
  renderMinerLight();
}


void GLCameraHandler::renderMinerLight()
{
  if(m_minerOn)
  {
    m_minerLight->configure();
    m_minerLight->render();
  }
}

GLLight * GLCameraHandler::getMinerLight() const
{
  return m_minerLight;
}

void GLCameraHandler::setMinerLight(GLLight * val )
{
  if(m_minerLight)
    delete m_minerLight;
  m_minerLight = val;
}

void GLCameraHandler::setMinerLightOn(bool val )
{
  m_minerOn = val;
}


void GLCameraHandler::setViewBoundingBox(Vector3 bb_min, Vector3 bb_max , float fovy)
{
  Vector3 bb_center =  (bb_max + bb_min)*.5;
  Vector3 bb_size = bb_max - bb_min;
  m_at = bb_center;

  float size = max(max(bb_size.x, bb_size.y), bb_size.z)/2;
  float r = (size/tan(DEG_TO_RAD(fovy/2)) + size)*1.2f;
  m_pos = m_at - Vector3(0.0f, 0.0f, r);

  Vector3 forward = (m_at-m_pos).unitary();
  if(!(forward == Vector3(0,1,0)))
  {
    Vector3 right = (forward^Vector3(0,1,0)).unitary();
    m_up = (right^forward).unitary();
  }else 
    m_up = Vector3(0,0,-1);
}

Vector3 GLCameraHandler::getPos() const
{
  return m_pos;
}

void GLCameraHandler::setPos( Vector3 val )
{
  m_pos = val;
}

Vector3 GLCameraHandler::getAt() const
{
  return m_at;
}

void GLCameraHandler::setAt( Vector3 val )
{
  m_at = val;
}

Vector3 GLCameraHandler::getUp() const
{
  return m_up;
}

void GLCameraHandler::setUp( Vector3 val )
{
  m_up = val;
}

