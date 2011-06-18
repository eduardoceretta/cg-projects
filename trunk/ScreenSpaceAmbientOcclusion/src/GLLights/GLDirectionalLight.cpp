/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  May 2008  
 *
 *  OpenGL Light utility class.
 *  Represents a Directional Light
 */
#include <iostream>

#include "GLLights/GLDirectionalLight.h"

GLDirectionalLight :: GLDirectionalLight():GLLight()
{
   m_pos[3] = 0.0;
}

GLDirectionalLight :: ~GLDirectionalLight()
{
}


void GLDirectionalLight :: render()
{
  GLLight :: render();
}

void GLDirectionalLight :: configure()
{
  GLLight :: configure();
}
