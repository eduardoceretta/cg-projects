/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Jul 2008  
 *
 *  Analytic LineXColumn Matrix3x3 math.
 */
#ifndef _MATRIX3_H_
#define _MATRIX3_H_

#include <iostream>
#include <math.h>


#include "MathUtils\MathDefines.h"
#include "MathUtils\Vector3.h"

class Matrix3
{
   /**
   * Matrix components
   */ 
  double m_mat[3][3];

public:
  /**
   * Simple Constructors
   */
  Matrix3();
  Matrix3(double m[3][3]);
  Matrix3(float m[3][3]);
  Matrix3(int m[3][3]);

  /**
   * Copy Constructor.
   */
  Matrix3(const Matrix3& m);

  /**
   * Set the values to the respective line
   */
  void setLine(const double l[3], int line);
  void setLine(const Vector3 &v, int line);

  /**
   * Set the values to the respective column
   */
  void setColumn(const double c[3], int column);
  void setColumn(const Vector3 &v, int column);

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
   * Creates a rotations matrix
   */
  inline void setRotation(float ang, Vector3 axis);

  /**
   * Assignment Operator
   */
  void operator= (const Matrix3& m);
  
  /**
   * Math Operators
   */
  void operator+= (const Matrix3& m);
  void operator-= (const Matrix3& m);
  void operator*= (const int val);
  void operator*= (const float val);
  void operator*= (const double val);
  void operator*= (const Matrix3& m);
  Matrix3 operator+(const Matrix3& m) const;
  Matrix3 operator-(const Matrix3& m) const;
  Matrix3 operator* (const int val) const;
  Matrix3 operator* (const float val) const;
  Matrix3 operator* (const double val) const;
  Matrix3 operator* (const Matrix3& m) const;
  inline Vector3 operator* (const Vector3& m) const;

  /**
   * Returns a Transpose Matrix
   */
  Matrix3 getTranspose() const;

  /**
  * Gets the determinant of the matrix
  */
  double det() const;
};


inline void Matrix3 :: setRotation(float ang, Vector3 axis)
{
   float cosA=cos(DEG_TO_RAD(ang)),sinA=sin(DEG_TO_RAD(ang));
   m_mat[0][0] =  axis.x*axis.x + (1-axis.x*axis.x)*cosA;
   m_mat[0][1] =  axis.x*axis.y*(1-cosA) -axis.z*sinA;
   m_mat[0][2] =  axis.x*axis.z*(1-cosA) +axis.y*sinA;

   m_mat[1][0] =  axis.x*axis.y*(1-cosA) + axis.z*sinA;
   m_mat[1][1] =  axis.y*axis.y + (1-axis.y*axis.y)*cosA;
   m_mat[1][2] =  axis.y*axis.z*(1-cosA) -axis.x*sinA;

   m_mat[2][0] =  axis.x*axis.z*(1-cosA) - axis.y*sinA;
   m_mat[2][1] =  axis.y*axis.z*(1-cosA) +axis.x*sinA;
   m_mat[2][2] =  axis.z*axis.z + (1-axis.z*axis.z)*cosA;
}


inline Vector3 Matrix3 :: operator* (const Vector3& v) const
{
   Vector3 aux(m_mat[0][0]*v.x + m_mat[0][1]*v.y + m_mat[0][2]*v.z,
               m_mat[1][0]*v.x + m_mat[1][1]*v.y + m_mat[1][2]*v.z,
               m_mat[2][0]*v.x + m_mat[2][1]*v.y + m_mat[2][2]*v.z    );
   return aux;
}


/**
 * Stream output operator
 */
std::ostream &operator<<(std::ostream & os, Matrix3 & m);


#endif

