/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Aug 2011 
 *
 *  Analytic Vector math.
 */

#include <cmath>

#include "Vector4.h"

Vector4 :: Vector4(int _x , int _y, int _z, int _w) 
:x((float)_x), y((float)_y), z((float)_z), w((float)_w)
{
}

Vector4 :: Vector4(float _x , float _y, float _z, float _w) 
:x(_x), y(_y), z(_z), w(_w)
{
}

Vector4 :: Vector4(double _x , double _y, double _z, double _w) 
:x((float)_x),  y((float)_y), z((float)_z), w((float)_w)
{
}

Vector4::Vector4( Vector3 v, float _w /*= 1.0f*/ )
:x(v.x), y(v.y), z(v.z), w(_w)
{
}

Vector4 :: Vector4(const Vector4& u)
:x(u.x), y(u.y), z(u.z), w(u.w)
{
}

void Vector4 :: operator+= (const Vector4& v)
{
  x += v.x; y += v.y; z += v.z; w += v.w;
}

void Vector4 :: operator -= (const Vector4& v)
{
  x-=v.x; y-=v.y; z-=v.z; w-=v.w;
}


void Vector4 :: operator*= (const int val)
{
  x*=val; y*=val; z*=val; w*=val;
}

void Vector4 :: operator*= (const float val)
{
  x*=val; y*=val; z*=val; w*=val;
}

void Vector4 :: operator*= (const double val)
{
  x*=float(val); y*=float(val); z*=float(val); w*=float(val);
}

Vector4 Vector4 :: operator + (const Vector4& v) const
{
  Vector4 aux( x + v.x, y + v.y, z + v.z , w + v.w);
  return( aux );
}

Vector4 Vector4 :: operator - (const Vector4& v) const
{
  Vector4 aux( x - v.x, y - v.y, z - v.z , w - v.w);
  return( aux );
}

double Vector4 :: operator* (const Vector4& v) const
{
  return x*v.x + y*v.y + z*v.z + w*v.w;
}

Vector4 Vector4 :: operator* (const int val) const
{
  Vector4 aux(x*val, y *val, z*val, w*val);
  return aux;
}

Vector4 Vector4 :: operator* (const float val) const
{
  Vector4 aux(x*val,y*val, z*val, w*val);
  return aux;
}

Vector4 Vector4 :: operator* (const double val) const
{
  Vector4 aux(x*val, y*val, z*val, w*val);
  return aux;
}

double Vector4 :: operator~() const
{
  return sqrt(x*x+y*y+z*z+w*w);
}

bool Vector4 :: operator== (const Vector4& v) const
{
  return x==v.x && y==v.y && z==v.z && w==v.w;
}

bool Vector4 :: operator< (const Vector4& v) const
{
  float vDist = ~(v- Vector4(0.0f, 0.0f, 0.0f,0.0f));
  float mDist = ~(*this - Vector4(0.0f, 0.0f, 0.0f,0.0f));
  if(mDist<vDist)
    return true;
  else
    return false;
}

Vector4 Vector4 :: unitary() const
{
  double norma = ~*this;
  Vector4 aux(x/norma, y/norma, z/norma, w/norma);
  return aux;
}

Vector4 Vector4::homogeneous() const
{
  return Vector4(x/w, y/w, z/w, 1.0f);
}

Vector3 Vector4::vec3() const
{
  return Vector3(x, y, z);
}

Vector3 Vector4::homogeneousVec3() const
{
  return Vector3(x/w, y/w, z/w);
}

std::ostream &operator<<(std::ostream & os, const Vector4 & v)
{
  return os   << "(" << v.x <<", " << v.y <<", " << v.z << ", " << v.w <<")"<<std::endl;
}
