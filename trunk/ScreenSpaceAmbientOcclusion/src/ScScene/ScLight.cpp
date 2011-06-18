/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Mar 2010
 *
 *  OpenGL Light Attributes Container
 */

#include <iostream>
#include <cmath>

#include "GLLights/GLLight.h"
#include "GLLights/GLPointLight.h"
#include "GLLights/GLDirectionalLight.h"
#include "GLLights/GLSpotLight.h"

#include "ScScene/ScLight.h"

using namespace std;

int ScLight :: s_lightNum = 0;

ScLight :: ScLight()
:m_calculed(false)
,m_lightStruct(NULL)
,m_GLLight(NULL)
{
   m_myLightNum = s_lightNum++;

   if(m_myLightNum >= Max_Lights)
      m_myLightNum = m_myLightNum%Max_Lights;
}

ScLight :: ~ScLight()
{
  if(m_lightStruct)
    delete m_lightStruct;
}

int ScLight :: getMyLightNumber() const
{
   return m_myLightNum;
}


void ScLight :: readFromStr(char buffer[])
{
  float type = (float)Point; 
  m_spotExponent = 0.0f;
  m_spotAngle = 180.0f;
  m_spotDir = Vector3(0,0,0);
  int r = sscanf( buffer, 
    "%f %f %f \
    %f %f %f \
    %f %f %f \
    %f \
    %f \
    %f \
    %f %f %f \n", 
      &m_pos.x, &m_pos.y, &m_pos.z,
      &m_diffuse.r, &m_diffuse.g, &m_diffuse.b, 
      &m_specular.r, &m_specular.g, &m_specular.b,
      &type,
      &m_spotExponent,
      &m_spotAngle,
      &m_spotDir.x, &m_spotDir.y, &m_spotDir.z);
   
  m_diffuse *= 1.0/255;
  m_specular *= 1.0/255;
  m_type = (LightType)(int) floor(type + .5);

  switch(m_type)
  {
    case Point:
      m_GLLight = new GLPointLight();
      break;
    case Directional:
      m_GLLight = new GLDirectionalLight();
      break;
    case Spot:
      m_GLLight = new GLSpotLight();
      break;
  }

  assert(r >= 9);
}

void ScLight :: configure()
{
   if(!m_calculed)
   {
      m_GLLight->setAmbientColor(m_diffuse*.2);
      m_GLLight->setDiffuseColor(m_diffuse);
      m_GLLight->setSpecularColor(m_specular);
      m_GLLight->setPosition(m_pos);
      if(m_type == Spot)
      {
        ((GLSpotLight*)m_GLLight)->setSpotAngle(m_spotAngle);
        ((GLSpotLight*)m_GLLight)->setSpotDirection(m_spotDir);
        ((GLSpotLight*)m_GLLight)->setSpotExponent(m_spotExponent);
      }
      m_calculed = true;
   }
   m_GLLight->configure();
}

void ScLight :: render()
{
   m_GLLight->render();
}

struct lightStruct * ScLight::getLightStruct()
{
  if(m_lightStruct)
    return m_lightStruct;

  m_lightStruct = new lightStruct;
  m_lightStruct->diffuse[0] = m_diffuse.r;
  m_lightStruct->diffuse[1] = m_diffuse.g;
  m_lightStruct->diffuse[2] = m_diffuse.b;
  m_lightStruct->spotExponent = m_spotExponent;
  m_lightStruct->specular[0] = m_specular.r;
  m_lightStruct->specular[1] = m_specular.g;
  m_lightStruct->specular[2] = m_specular.b;
  m_lightStruct->enabled = 1.0;
  m_lightStruct->pos[0] = m_pos.x;
  m_lightStruct->pos[1] = m_pos.y;
  m_lightStruct->pos[2] = m_pos.z;
  m_lightStruct->type = (float)(int)m_type;
  m_lightStruct->spotDir[0] = m_spotDir.x;
  m_lightStruct->spotDir[1] = m_spotDir.y;
  m_lightStruct->spotDir[2] = m_spotDir.z;
  m_lightStruct->spotAngle = DEG_TO_RAD(m_spotAngle);
  return m_lightStruct;
}



