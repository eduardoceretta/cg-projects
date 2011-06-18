/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  May 2008  
 *
 *  OpenGL Light utility class.
 *  Represents a Ponit Light
 */
#ifndef _GL_POINT_LIGHT_H_
#define _GL_POINT_LIGHT_H_

#include <string>
#include <GL/glut.h>

#include "GLLights/GLLight.h"

class GLPointLight : public GLLight
{
public:
  /**
   * Create a Point Light
   */
  GLPointLight();
  
  /**
   * Destroy a Point Light
   */
  ~GLPointLight();

  /**
   * Base Calls
   */
  virtual void render();
  virtual void configure();
};

#endif




