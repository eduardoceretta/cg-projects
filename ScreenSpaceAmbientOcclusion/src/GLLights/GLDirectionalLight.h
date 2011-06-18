/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  May 2008  
 *
 *  OpenGL Light utility class.
 *  Represents a Directional Light
 */
#ifndef _GL_DIRECTIONAL_LIGHT_H_
#define _GL_DIRECTIONAL_LIGHT_H_

#include <string>

#include "GLLights/GLLight.h"

using namespace std;

class GLDirectionalLight : public GLLight
{
public:
  /**
   * Create a Directional Light
   */
  GLDirectionalLight();

  /**
   * Destroy a Directional Light
   */
  ~GLDirectionalLight();

  /**
   * Base Calls
   */
  virtual void render();
  virtual void configure();
};


#endif





