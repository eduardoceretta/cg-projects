/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Aug 2011  
 *
 *  Analytic Vector math.
 */
#ifndef _VECTOR4_H_
#define _VECTOR4_H_

#include <iostream>

#include "MathUtils/MathDefines.h"
#include "MathUtils/Vector3.h"

class Vector4
{
public:
  /**
   * Vector components
   */
  float x, y, z, w;

  /**
   * Simple Constructor
   */
  inline Vector4() {}
  
  /**
   * float Constructor
   */
  Vector4(float x , float y, float z, float w);

  /**
   * double Constructor. Converts to float
   */
  Vector4(double x , double y, double z, double w);
  
  /**
   * int Constructor. Converts to float
   */
  Vector4(int x , int y, int z, int w);

  /**
   * Vector3 Constructor.
   */
  Vector4(Vector3 v, float w = 1.0f);
  
  /**
   * Copy Constructor.
   */
  Vector4(const Vector4& u);

  /**
   * Vector sum operators.
   */
  void operator+= (const Vector4& v);
  Vector4 operator+(const Vector4& v) const;
  
  /**
   * Vector diff operators.
   */
  void operator-= (const Vector4& v);
  Vector4 operator-(const Vector4& v) const;

  /**
   * Vector scalar product operators.
   */
  void operator*= (const int val);
  void operator*= (const float val);
  void operator*= (const double val);
  Vector4 operator* (const int val) const;
  Vector4 operator* (const float val) const;
  Vector4 operator* (const double val) const;

  /**
   * Vector dot product operator.
   */
  double operator* (const Vector4& v) const;

  /**
   * Vector length operator.
   */
  double operator~() const;

  /**
   * Vector comparisons operators.
   */
  bool operator== (const Vector4& v) const;
  bool operator< (const Vector4& v) const;

  /**
   * Vector normalization.
   *  Returns the normalized vector.
   */
  Vector4 unitary() const;

  /**
   * Vector homogeneous.
   *  Divides by w
   */
  Vector4 homogeneous() const;

  /**
   * Return a Vec3 with with x, y, z
   *  W is ignored
   */
  Vector3 vec3() const;
  
  /**
   * Return a Vec3 in homogeneous coordinates
   *  Divides by w
   */
  Vector3 homogeneousVec3() const;
};

/**
 * Stream output operator
 */
std::ostream &operator<<(std::ostream & os, const Vector4 & v);

#endif

