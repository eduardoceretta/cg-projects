/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
                  Matricula:2610166
\**********************************************************/
/**********************************************************\
                  Cabeçalho Matrix4
   Contem a definicao da classe Matrix4
\**********************************************************/
#ifndef _MATRIX4_H_
#define _MATRIX4_H_

#include <iostream>
#include "Vector3.h"


class Matrix4
{
   double mat[4][4];
   public:
      Matrix4();
      Matrix4(double m[4][4]);
      Matrix4(float m[4][4]);
      Matrix4(int m[4][4]);

      Matrix4(const Matrix4& m);

      void setLine(const double l[4], int line);
      void setColumn(const double c[4], int column);
      void setValue(const double val, int line, int column);

      double getValue(int line, int column);

      void setIdentity();
      void setRotationMatrix(double mAng, Vector3 ax);

      void operator= (const Matrix4& m);

      void operator+= (const Matrix4& m);
      void operator-= (const Matrix4& m);
      void operator*= (const int val);
      void operator*= (const float val);
      void operator*= (const double val);
      void operator*= (const Matrix4& m);


      Matrix4 operator* (const int val) const;
      Matrix4 operator* (const float val) const;
      Matrix4 operator* (const double val) const;
      Matrix4 operator* (const Matrix4& m) const;
      Vector3 operator* (const Vector3& m) const;


      Matrix4 operator+(const Matrix4& m) const;
      Matrix4 operator-(const Matrix4& m) const;

      Matrix4 operator~() const;

};

std::ostream &operator<<(std::ostream & os, Matrix4 & m);


#endif

