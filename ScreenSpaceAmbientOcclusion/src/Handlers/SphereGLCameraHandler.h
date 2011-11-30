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
#include "Handlers/GLCameraHandler.h"

class GLLight;

class SphereGLCameraHandler : public GLCameraHandler
{
protected:
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

public:
  /**
   * Construct the handler and initialize the sphere attributes
   */
  SphereGLCameraHandler(float sphereRadius = 100.0f, float sphereAlpha = 0.0f, float sphereBeta = 0.0f,  float inc = 5.0f);
  ~SphereGLCameraHandler();
  
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

  float getSphereAlpha() const;
  float getSphereBeta() const;
  float getSphereRadius() const;
  Vector3 getPos() const;

  /**
   * Render an OpenGL Camera
   *  Render the light if it is on
   */
  void render();

  /**
   * Make the sphere centered in the bounding box with the appropriated radius size
   */
  void setViewBoundingBox(Vector3 bb_min, Vector3 bb_max , float fovy);
};




