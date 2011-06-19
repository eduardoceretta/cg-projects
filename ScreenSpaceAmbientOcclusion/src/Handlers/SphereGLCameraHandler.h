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
   * Increment of cammera movements
   */
  float m_inc;

  /**
   * Mouse Controll Atrributes
   */
  int m_lastMousePosX;
  int m_lastMousePosY;
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
  void listenKeyboard(int key);
  void listenMouseMove(int x, int y);
  void listenMouseClick(int button, int state, int x, int y);

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
};




