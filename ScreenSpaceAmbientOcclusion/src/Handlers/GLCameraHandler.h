/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Nov 2011
 *
 *  Camera Handler. Simple OpenGl LookAt Camera
 */
#pragma once

#include <gl/glut.h>
#include "MathUtils/Vector3.h"

class GLLight;

class GLCameraHandler
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
  GLCameraHandler();
  GLCameraHandler(Vector3 pos, Vector3 at, Vector3 up);
  virtual ~GLCameraHandler();

  /**
   *  FUTURE WORK
   * Read the attributes from a space separated string.
   *  String format: r a b
   */
  //void readFromStr(char buffer[]);

  /**
   * Interface Listeners
   */
  virtual void listenSpecialKeyboard(int key);
  virtual void listenMouseMove(float x, float y);
  virtual void listenMouseClick(int button, int state, float x, float y);

  /**
   * Sphere parameters getters and setters.
   */
  virtual void setPos(Vector3 val);
  virtual void setAt(Vector3 val);
  virtual void setUp(Vector3 val);

  virtual Vector3 getPos() const;
  virtual Vector3 getAt() const;
  virtual Vector3 getUp() const;


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
  virtual void render();

  /**
   * Render only the light if it is on
   */
  void renderMinerLight();

  /**
   * Make the sphere centered in the bounding box with the appropriated radius size
   */
  virtual void setViewBoundingBox(Vector3 bb_min, Vector3 bb_max , float fovy);
};




