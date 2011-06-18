/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  May 2008  
 *
 *  OpenGL Light utility class.
 *  Abstract class represents the basis of an OpenGL Light
 */
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>

#include "GLLights/GLLight.h"

using namespace std;

int GLLight :: s_lightNum = 0;

/**
 * Max Lights Accepted
 */
#define MY_MAX_LIGHT 8

GLLight :: GLLight()
{
   m_myLightNum = (GL_LIGHT0+s_lightNum++);

   if(m_myLightNum >=(GL_LIGHT0+MY_MAX_LIGHT))
   {
      m_myLightNum = m_myLightNum%(GL_LIGHT0+MY_MAX_LIGHT)+GL_LIGHT0;
   }

   for (unsigned i(0); i < 4; ++i)
   {
      m_pos[i] = 0;
      m_ambient[i] = 0.1;
      m_specular[i] = 1;
      m_diffuse[i] = 1;
   }
   m_ambient[3] = 1;

   m_constAttenuation = 1.0;
   m_linAttenuation = 0.0;
   m_quadAttenuation = .0;

   m_turnedOn = true;
   m_modified = true;
}

GLLight :: ~GLLight()
{
   glDisable(m_myLightNum);
}

void GLLight :: render()
{
   if(m_turnedOn)
   {
      glEnable(GL_LIGHTING);
      glEnable(m_myLightNum);
      glLightfv(m_myLightNum, GL_POSITION, m_pos); 
   }
   else glDisable(m_myLightNum);
}
void GLLight :: configure()
{
   if(m_modified && m_turnedOn)
   {
      glLightfv(m_myLightNum, GL_AMBIENT,  m_ambient);
      glLightfv(m_myLightNum, GL_DIFFUSE,  m_diffuse);
      glLightfv(m_myLightNum, GL_SPECULAR, m_specular);
      glLightf(m_myLightNum, GL_CONSTANT_ATTENUATION, m_constAttenuation);
      glLightf(m_myLightNum, GL_LINEAR_ATTENUATION, m_linAttenuation);
      glLightf(m_myLightNum, GL_QUADRATIC_ATTENUATION, m_quadAttenuation);
      m_modified = false;
   }
}

void GLLight :: setPosition(Vector3 position)
{
   m_pos[0] = position.x;
   m_pos[1] = position.y;
   m_pos[2] = position.z;
   m_modified = true;
}

Vector3 GLLight :: getPosition() const
{
   return Vector3(m_pos[0],m_pos[1],m_pos[2]);
}


void GLLight :: setAmbientColor(Color c)
{
   m_ambient[0] = c.r;
   m_ambient[1] = c.g;
   m_ambient[2] = c.b;
   m_modified = true;
}

Color GLLight :: getAmbientColor() const
{
   return Color(m_ambient[0],m_ambient[1],m_ambient[2]);
}


void GLLight :: setSpecularColor(Color c)
{
   m_specular[0] = c.r;
   m_specular[1] = c.g;
   m_specular[2] = c.b;
   m_modified = true;
}

Color GLLight :: getSpecularColor() const
{
   return Color(m_specular[0],m_specular[1],m_specular[2]);
}


void GLLight :: setDiffuseColor(Color c)
{
   m_diffuse[0] = c.r;
   m_diffuse[1] = c.g;
   m_diffuse[2] = c.b;
   m_modified = true;
}

Color GLLight :: getDiffuseColor() const
{
   return Color(m_diffuse[0],m_diffuse[1],m_diffuse[2]);
}


void GLLight :: lightTurnedOn(bool op)
{
   m_turnedOn = op;
}

bool GLLight :: lightIsTurnedOn() const
{
   return m_turnedOn;
}


void GLLight :: setAtenuation(double constant, double linear, double quadric)
{
   m_constAttenuation = constant;
   m_linAttenuation = linear;
   m_quadAttenuation = quadric;
   m_modified = true;
}

void GLLight :: getAtenuation(double &constant, double &linear, double &quadric)  const
{
   constant = m_constAttenuation ;
   linear = m_linAttenuation;
   quadric = m_quadAttenuation;
}


int GLLight :: getMyLightNumber() const
{
   return m_myLightNum - GL_LIGHT0;
}



