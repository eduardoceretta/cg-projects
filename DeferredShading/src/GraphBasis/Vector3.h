/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
                  Matricula:2610166
\**********************************************************/
/**********************************************************\
                  Cabeçaho Vector3
   Contem a definicao dasclasse Vector3
\**********************************************************/
#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#include <iostream>

#include "GraphBasis\GraphBasisDefines.h"

class Vector3
{
   public:
      float x,y,z;
      typedef enum { X,Y,Z} Axis;
      Vector3() {}
      Vector3(float _x , float _y, float _z);
      Vector3(double _x , double _y, double _z);
      Vector3(int _x , int _y, int _z);

      Vector3(const Vector3& u);

      void operator+= (const Vector3& v);
      void operator-= (const Vector3& v);
      void operator^= (const Vector3& v);
      void operator*= (const int val);
      void operator*= (const float val);
      void operator*= (const double val);


      Vector3 operator* (const int val) const;
      Vector3 operator* (const float val) const;
      Vector3 operator* (const double val) const;


      Vector3 operator+(const Vector3& v) const;
      Vector3 operator-(const Vector3& v) const;
      Vector3 operator^(const Vector3& v) const;

      double operator* (const Vector3& v) const;
      double operator~() const;

      bool operator== (const Vector3& v) const;
      bool operator< (const Vector3& v) const;

      Vector3 unitary() const;
      double angle(const Vector3& v) const;
//      double angleVerified(const Vector3& v,Axis ax) const;
		void 	 rotateTo(const double angle,Axis ax);
		void 	 rotate(const double angle,Axis ax);
		void 	 rotateAxis(const double angle,Vector3 ax);
};

std::ostream &operator<<(std::ostream & os, const Vector3 & v);


#endif

