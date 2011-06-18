/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  May 2008  
 *
 *  OpenGL Light utility class.
 *  Represents a Spot Light
 */
#ifndef _GL_SPOT_LIGHT_H_
#define _GL_SPOT_LIGHT_H_

#include "GLLights/GLLight.h"
#include "MathUtils/Vector3.h"

class GLSpotLight : public GLLight
{
protected:
  /**
   * Spot's Direction
   */
  GLfloat m_direction[3];

  /**
   * Spot's Opening Angle 
   *  [0 - 90]
   */
  double m_cutOfAng;

  /**
   * Spot's border attenuation.
   */
  double m_exponentValue;

public:
  /**
   * Create a Point Light
   */
  GLSpotLight();

  /**
   * Destroy a Directional Light
   */
  ~GLSpotLight();

  /**
   * Enable OpenGL Light State.
   *  Set the light position and direction so it can be transformed by the current modelview
   */
  virtual void render();

  /**
   * Update Light Attributes if needed
   */
  virtual void configure();

 /**
   * Attributes Getters and Setters
   */
  void setSpotAngle(double ang);
  double getSpotAngle()const;

  void setSpotExponent(double exp);
  double getSpotExponent()const;

  void setSpotDirection(Vector3 dir);
  Vector3 getSpotDirection()const;
};


#endif





