/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  May 2008  
 *
 *  OpenGL Light utility class.
 *  Represents a Spot Light
 */
#include <iostream>

#include "GLLights/GLSpotLight.h"
GLSpotLight :: GLSpotLight():GLLight()
{
   memset(m_direction, 0, 3*sizeof(GLfloat));
   m_pos[3] = 1.0;
   m_cutOfAng = 45.0;
   m_exponentValue = 0.0;
}

GLSpotLight :: ~GLSpotLight()
{
}


void GLSpotLight :: render()
{
   glLightfv(m_myLightNum, GL_SPOT_DIRECTION, m_direction);
   GLLight :: render();
}

void GLSpotLight :: configure()
{
   GLLight::configure();
   glLightf(m_myLightNum, GL_SPOT_CUTOFF, m_cutOfAng);
   glLightf(m_myLightNum, GL_SPOT_EXPONENT, m_exponentValue);
}


void GLSpotLight :: setSpotAngle(double ang)
{
   m_cutOfAng = ang;
   m_modified = true;
}

double GLSpotLight :: getSpotAngle()const
{
   return m_cutOfAng;
}

void GLSpotLight :: setSpotExponent(double exp)
{
   m_exponentValue = exp;
   m_modified = true;
}

double GLSpotLight :: getSpotExponent() const
{
   return m_exponentValue;
}

void GLSpotLight :: setSpotDirection(Vector3 dir)
{
   m_direction[0] = dir.x;
   m_direction[1] = dir.y;
   m_direction[2] = dir.z;
   m_modified = true;
}

Vector3 GLSpotLight :: getSpotDirection() const
{
   return Vector3(m_direction[0],m_direction[1],m_direction[2]);
}

