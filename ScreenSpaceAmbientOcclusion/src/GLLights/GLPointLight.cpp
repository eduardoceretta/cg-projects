/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  May 2008  
 *
 *  OpenGL Light utility class.
 *  Represents a Ponit Light
 */

#include <iostream>
#include <GL/glut.h>

#include "GLLights/GLPointLight.h"

GLPointLight :: GLPointLight():GLLight()
{
   m_pos[3] = 1.0;
}

GLPointLight :: ~GLPointLight()
{
}


void GLPointLight :: render()
{
   GLLight :: render();
}

void GLPointLight :: configure()
{
   GLLight :: configure();
}
