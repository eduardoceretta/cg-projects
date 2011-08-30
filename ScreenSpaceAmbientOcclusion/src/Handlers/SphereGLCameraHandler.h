/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  May 2011
 *
 *  Camera Handler. Position the camera in the hull of
 *  a sphere looking at its center.
 */
#pragma once

#include <gl/glut.h>
#include "MathUtils/Vector3.h"

class GLLight;

class SphereGLCameraHandler
{
protected:
  /**
   * Camera World Position
   */
  Vector3 m_pos;

  /**
   * Camera LookAt Position
   */
  Vector3 m_at;

  /**
   * Camera Up Vector
   */
  Vector3 m_up;

  /**
   * Sphere Radius
   */
  float m_r;
  
  /**
   * Sphere alpha angle
   */
  float m_alpha;

  /**
   * Sphere beta angle
   */
  float m_beta;

  /**
   * Increment of camera movements
   */
  float m_inc;
  float m_rinc;

  /**
   * Mouse control Attributes
   */
  float m_lastMousePosX;
  float m_lastMousePosY;
  int m_mouseState;
  int m_mouseButton;
  
  /**
   * Keyboard modifier key (GLUT_ACTIVE_SHIFT...)
   */
  int m_modifier;

  /**
   * Light always at the camera position
   */
  GLLight *m_minerLight;

  /**
   * Indicates if the Miner Light is On
   */
  bool m_minerOn;
  
public:
  /**
   * Construct the handler and initialize the sphere attributes
   */
  SphereGLCameraHandler(float sphereRadius = 100.0f, float sphereAlpha = 0.0f, float sphereBeta = 0.0f,  float inc = 5.0f);
  
  /**
   *  FUTURE WORK
   * Read the attributes from a space separated string.
   *  String format: r a b
   */
  //void readFromStr(char buffer[]);

  /**
   * Interface Listeners
   */
  void listenSpecialKeyboard(int key);
  void listenMouseMove(float x, float y);
  void listenMouseClick(int button, int state, float x, float y);

  /**
   * Sphere parameters getters and setters.
   */
  void setSphereAlpha(float alpha);
  void setSphereBeta(float beta);
  void setSphereRadius(float radius);
  void setPos(Vector3 val);
  void setAt(Vector3 val);
  void setUp(Vector3 val);

  float getSphereAlpha() const;
  float getSphereBeta() const;
  float getSphereRadius() const;
  Vector3 getPos() const;
  Vector3 getAt() const;
  Vector3 getUp() const;


  /**
   * Get the Miner Light Pointer
   */ 
  GLLight* getMinerLight() const;

  /**
   * Set the Miner Light Pointer.
   *  Deletes the previous allocated light
   */
  void setMinerLight(GLLight * val);

  /**
   * Turn on/off the miner light
   */
  void setMinerLightOn(bool val);

  /**
   * Render an OpenGL Camera
   *  Render the light if it is on
   */
  void render();

  /**
   * Render only the light if it is on
   */
  void renderMinerLight();

  /**
   * Make the sphere centered in the bounding box with the appropriated radius size
   */
  void setViewBoundingBox(Vector3 bb_min, Vector3 bb_max , float fovy);
};




