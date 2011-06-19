/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Mar 2010
 *
 *  OpenGL Camera Attributes Container
 */
#pragma once

#include "MathUtils/Vector3.h"

class ScCamera
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
   * Field of view
   */
  float m_fovy;

  /**
   * Camera Near Plane
   */
  float m_near;

  /**
   * Camera Far Plane
   */
  float m_far;

  /**
   * Screen Width
   */
  int m_screenWidth;

  /**
   * Screen Height
   */
  int m_screenHeight;


public:
  /**
   * Simple Constructor
   */
  ScCamera(){}

  /**
   * Direct Constructor
   */
  ScCamera(Vector3 pos, Vector3 at, Vector3 up, float fovy,
          float near, float far, int screenWidth, int screenHeight);
  
  /**
   * Read the attributes from a space separated string.
   *  String format: pos.x pos.y pos.z at.x at.y at.z up.x up.y up.z fovy near far screenWidth screenHeight
   */
  void readFromStr(char buffer[]);

  /**
   * Getters of Camera Attributes
   */
  Vector3 getPos() const;
  Vector3 getAt() const;
  Vector3 getUp() const;
  float getFovy() const;
  float getNear() const;
  float getFar() const;
  int getScreenWidth() const;
  int getScreenHeight() const;
};




