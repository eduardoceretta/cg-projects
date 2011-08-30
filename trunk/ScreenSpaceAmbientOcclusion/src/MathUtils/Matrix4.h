/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Jul 2008  
 *
 *  Analytic LineXColumn Matrix4x4 math.
 */
#ifndef _MATRIX4_H_
#define _MATRIX4_H_

#include <iostream>
#include "MathUtils\Vector3.h"
#include "MathUtils\Vector4.h"

class Matrix4
{
  /**
   * Matrix components
   */
  double m_mat[4][4];

public:
  /**
   * Simple Constructors
   */
  Matrix4();
  Matrix4(double m[4][4]);
  Matrix4(float m[4][4]);
  Matrix4(int m[4][4]);
  Matrix4(float *m);
  
  /**
   * Copy Constructor.
   */
  Matrix4(const Matrix4& m);

  /**
   * Set the values to the respective line
   */
  void setLine(const double l[4], int line);

  /**
   * Set the values to the respective column
   */
  void setColumn(const double c[4], int column);

  /**
   * Set the value to the respective position
   */
  void setValue(const double val, int line, int column);

  /**
   * Get the value from the matrix
   */
  double getValue(int line, int column);

  /**
   * Set the matrix as Identity
   */
  void setIdentity();

  /**
   * Inverset the current matrix
   */
  void Invert();

  /**
   * Transpose the current matrix
   */
  void Transpose();

  /**
   * Creates a rotations matrix
   */
  void setRotationMatrix(double mAng, Vector3 ax);

  /**
   * Assignment Operator
   */
  void operator= (const Matrix4& m);

  /**
   * Math Operators
   */
  void operator+= (const Matrix4& m);
  void operator-= (const Matrix4& m);
  void operator*= (const int val);
  void operator*= (const float val);
  void operator*= (const double val);
  void operator*= (const Matrix4& m);

  Matrix4 operator+(const Matrix4& m) const;
  Matrix4 operator-(const Matrix4& m) const;
  Matrix4 operator* (const int val) const;
  Matrix4 operator* (const float val) const;
  Matrix4 operator* (const double val) const;
  Matrix4 operator* (const Matrix4& m) const;
  Vector4 operator* (const Vector4& v) const;
  
  /**
   * Considers only the Vector3 as a Vec4 with w = 1
   */  
  Vector3 operator* (const Vector3& m) const;

  /**
   * Considers only the Vector3 as a Vec4 with w = 1
   */  


  Vector3 mulVec4 (const Vector3& m, float w, float &outw) const;
};

/**
 * Stream output operator
 */
std::ostream &operator<<(std::ostream & os, Matrix4 & m);

#endif

