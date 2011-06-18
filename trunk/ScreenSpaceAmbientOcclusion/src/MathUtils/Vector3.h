/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Jul 2008  
 *
 *  Analytic Vector math.
 */
#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#include <iostream>

#include "MathUtils\MathDefines.h"

class Vector3
{
public:
  /**
   * Vector components
   */
  float x,y,z;

  /**
   * Definition of Axis for rotations.
   *  see rotateTo, rotate
   */
  typedef enum {X,Y,Z} Axis;

  /**
   * Simple Constructor
   */
  Vector3() {}
  
  /**
   * float Constructor
   */
  Vector3(float x , float y, float z);

  /**
   * double Constructor. Converts to float
   */
  Vector3(double x , double y, double z);
  
  /**
   * int Constructor. Converts to float
   */
  Vector3(int x , int y, int z);
  
  /**
   * Copy Constructor.
   */
  Vector3(const Vector3& u);

  /**
   * Vector sum operators.
   */
  void operator+= (const Vector3& v);
  Vector3 operator+(const Vector3& v) const;
  
  /**
   * Vector diff operators.
   */
  void operator-= (const Vector3& v);
  Vector3 operator-(const Vector3& v) const;

  /**
   * Vector cross product operators.
   */
  void operator^= (const Vector3& v);
  Vector3 operator^(const Vector3& v) const;
  
  /**
   * Vector scalar product operators.
   */
  void operator*= (const int val);
  void operator*= (const float val);
  void operator*= (const double val);
  Vector3 operator* (const int val) const;
  Vector3 operator* (const float val) const;
  Vector3 operator* (const double val) const;

  /**
   * Vector dot product operator.
   */
  double operator* (const Vector3& v) const;

  /**
   * Vector length operator.
   */
  double operator~() const;

  /**
   * Vector comparisons operators.
   */
  bool operator== (const Vector3& v) const;
  bool operator< (const Vector3& v) const;

  /**
   * Vector normalization.
   *  Returns the normalized vector.
   */
  Vector3 unitary() const;

  /**
   * Return the angle between two Vectors.
   *  Return in degrees.
   */
  double angle(const Vector3& v) const;

  /**
  * Rotates the vector to the vector witch has the given degree angle with the given cartesian axis.
  */
  void rotateTo(const double angle, Axis ax);

  /**
   * Rotates the vector around the cartesian axis angle degrees.
   */
  void 	 rotate(const double angle,Axis ax);

  /**
   * Rotates the vector around any given axis angle degrees.
   */
  void 	 rotateAxis(const double angle,Vector3 ax);
};

/**
 * Stream output operator
 */
std::ostream &operator<<(std::ostream & os, const Vector3 & v);

#endif

