/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  May 2008  
 *
 *  OpenGL Light utility class.
 *  Abstract class represents the basis of an OpenGL Light
 */
#ifndef _GL_LIGHT_H_
#define _GL_LIGHT_H_

#include <GL/glut.h>

#include "MathUtils/Vector3.h"
#include "MathUtils/Color.h"

class GLLight
{
  /**
   * Static number of lights counter.
   *  Only counts the number of created lights, do not consider deleted lights
   */
  static int s_lightNum;

protected:
  /**
   * Light World Position
   */
  GLfloat m_pos[4];
  
  /**
   * Light Color Attributes
   */
  GLfloat m_ambient[4];
  GLfloat m_diffuse[4];
  GLfloat m_specular[4];

  /**
   * Light Attenuation Attributes 
   */
  double m_constAttenuation;
  double m_quadAttenuation;
  double m_linAttenuation;

  /**
   * State of the light
   */
  bool m_turnedOn;

  /**
   * Indicate if the light need to be updated
   */
  bool m_modified;

  /**
   * This instance light number
   */
  int m_myLightNum;

public:
  /**
   * Simple Constructor, defines GL_LIGHTi
   */
  GLLight();

  /**
   * Disable this GL_LIGHTi
   */
  virtual ~GLLight() = 0;

  /**
   * Enable OpenGL Light State.
   *  Set the light position so it can be transformed by the current modelview
   */
  virtual void render();

  /**
   * Update Light Attributes if needed
   */
  virtual void configure();

  /**
   * Attributes Getters and Setters
   */
  void setPosition(Vector3 pos);
  Vector3 getPosition() const;

  void setAmbientColor(Color c);
  Color getAmbientColor() const;

  void setSpecularColor(Color c);
  Color getSpecularColor() const;

  void setDiffuseColor(Color c);
  Color getDiffuseColor() const;

  void setAtenuation(double constant, double linear, double quadric); 
  void getAtenuation(double &constant, double &linear, double &quadric) const;

  void lightTurnedOn(bool op); 
  bool lightIsTurnedOn() const;
  
  /**
   * Return the number of the light.
   *  Start at 0.
   *  If wanted the GL_LIGHTi sum with GL_LIGHT0
   */
  int getMyLightNumber() const;
};



#endif



