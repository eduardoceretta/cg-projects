/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
                  Matricula:2610166
\**********************************************************/
/**********************************************************\
                  Cabeçaho Matrix3
   Contem a definicao da classe Matrix3
\**********************************************************/
#ifndef _MATRIX3_H_
#define _MATRIX3_H_

#include <iostream>
#include <math.h>

#include "GraphBasisDefines.h"
#include "Vector3.h"

class Matrix3
{
   double mat[3][3];
   public:
      Matrix3();
      Matrix3(double m[3][3]);
      Matrix3(float m[3][3]);
      Matrix3(int m[3][3]);

      Matrix3(const Matrix3& m);

      void setLine(const double l[3], int line);
      void setLine(const Vector3 &v, int line);
      void setColumn(const double c[3], int column);
      void setColumn(const Vector3 &v, int column);
      void setValue(const double val, int line, int column);

      double getValue(int line, int column);

      void setIdentity();
      inline void setRotation(float ang, Vector3 axis);

      void operator= (const Matrix3& m);

      void operator+= (const Matrix3& m);
      void operator-= (const Matrix3& m);
      void operator*= (const int val);
      void operator*= (const float val);
      void operator*= (const double val);
      void operator*= (const Matrix3& m);


      Matrix3 operator* (const int val) const;
      Matrix3 operator* (const float val) const;
      Matrix3 operator* (const double val) const;
      Matrix3 operator* (const Matrix3& m) const;
      inline Vector3 operator* (const Vector3& m) const;


      Matrix3 operator+(const Matrix3& m) const;
      Matrix3 operator-(const Matrix3& m) const;

      Matrix3 operator~() const;

      double det() const;
};


inline void Matrix3 :: setRotation(float ang, Vector3 axis)
{
   float cosA=cos(DEG_TO_RAD(ang)),sinA=sin(DEG_TO_RAD(ang));
   mat[0][0] =  axis.x*axis.x + (1-axis.x*axis.x)*cosA;
   mat[0][1] =  axis.x*axis.y*(1-cosA) -axis.z*sinA;
   mat[0][2] =  axis.x*axis.z*(1-cosA) +axis.y*sinA;

   mat[1][0] =  axis.x*axis.y*(1-cosA) + axis.z*sinA;
   mat[1][1] =  axis.y*axis.y + (1-axis.y*axis.y)*cosA;
   mat[1][2] =  axis.y*axis.z*(1-cosA) -axis.x*sinA;

   mat[2][0] =  axis.x*axis.z*(1-cosA) - axis.y*sinA;
   mat[2][1] =   axis.y*axis.z*(1-cosA) +axis.x*sinA;
   mat[2][2] =  axis.z*axis.z + (1-axis.z*axis.z)*cosA;
}


inline Vector3 Matrix3 :: operator* (const Vector3& v) const
{
   Vector3 aux(mat[0][0]*v.x + mat[0][1]*v.y + mat[0][2]*v.z,
               mat[1][0]*v.x + mat[1][1]*v.y + mat[1][2]*v.z,
               mat[2][0]*v.x + mat[2][1]*v.y + mat[2][2]*v.z    );
   return aux;
}




std::ostream &operator<<(std::ostream & os, Matrix3 & m);


#endif

