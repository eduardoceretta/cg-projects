/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
                  Matricula:2610166
\**********************************************************/
/**********************************************************\
                  Arquivo Vector3
   Contem a implementaçao dos metodos da classe Vector3
\**********************************************************/

#include <cmath>

#include "GraphBasis\Vector3.h"

///////////////////
//~ Vector3
//////////////////
Vector3 :: Vector3(int _x , int _y, int _z) : x((float)_x),  y((float)_y), z((float)_z)
{
}

Vector3 :: Vector3(float _x , float _y, float _z) : x(_x),  y(_y), z(_z)
{
}

Vector3 :: Vector3(double _x , double _y, double _z) : x((float)_x),  y((float)_y), z((float)_z)
{
}

Vector3 :: Vector3(const Vector3& u)
{
   x = u.x; y = u.y; z = u.z;
}

void Vector3 :: operator+= (const Vector3& v)
{
   x += v.x; y += v.y; z += v.z;
}

void Vector3 :: operator -= (const Vector3& v)
{
   x-=v.x; y-=v.y; z-=v.z;
}

void Vector3 :: operator^= (const Vector3& v)
{
   float _x = (y * v.z - z * v.y);
   float _y = (z * v.x - x * v.z);
   z = (x * v.y - y * v.x);
   x=_x;
   y=_y;
}

void Vector3 :: operator*= (const int val)
{
   x*=val; y*=val; z*=val;
}

void Vector3 :: operator*= (const float val)
{
   x*=val; y*=val; z*=val;
}

void Vector3 :: operator*= (const double val)
{
   x*=val; y*=val; z*=val;
}

Vector3 Vector3 :: operator + (const Vector3& v) const
{
   Vector3 aux( x + v.x, y + v.y, z + v.z );
   return( aux );
}

Vector3 Vector3 :: operator - (const Vector3& v) const
{
   Vector3 aux( x - v.x, y - v.y, z - v.z );
   return( aux );
}

Vector3 Vector3 :: operator ^ (const Vector3& v) const
{
   Vector3 aux( y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x );
   return( aux );
}

double Vector3 :: operator* (const Vector3& v) const
{
   return x*v.x + y*v.y + z*v.z;
}

Vector3 Vector3 :: operator* (const int val) const
{
   Vector3 aux(x*val,y*val, z*val);
   return aux;
}

Vector3 Vector3 :: operator* (const float val) const
{
   Vector3 aux(x*val,y*val,z*val);
   return aux;
}

Vector3 Vector3 :: operator* (const double val) const
{
   Vector3 aux(x*val,y*val,z*val);
   return aux;
}

double Vector3 :: operator~() const
{
   return sqrt(x*x+y*y+z*z);
}

bool Vector3 :: operator== (const Vector3& v) const
{
   return x==v.x && y==v.y && z==v.z;
}

bool Vector3 :: operator< (const Vector3& v) const
{
   float vDist = ~(v- Vector3(0,0,0));
   float mDist = ~(*this - Vector3(0,0,0));
   if(mDist<vDist)
      return true;
   else
      return false;
}

Vector3 Vector3 :: unitary() const
{
   double norma = ~*this;
   Vector3 aux(x/norma, y/norma, z/norma);
   return aux;
}

double Vector3 :: angle(const Vector3& v) const
{
   Vector3 p = unitary();
   Vector3 q = v.unitary();

   double cosAngle = p*q;

   return RAD_TO_DEG(acos(clamp<double>(cosAngle,-1,1)));

//   return  (atan2(y,x) - atan2(v.y,v.x));
}

void 	 Vector3 :: rotateTo(const double angle,Axis ax)
{
	double ang = DEG_TO_RAD(angle);
   float xx,yy,zz;

	switch(ax)
	{
		case X:
			yy = y*y;
			zz = z*z;
			y = cos(ang)*sqrt(yy+zz);
			z = sin(ang)*sqrt(yy+zz);
		break;
		case Y:
         xx=x*x;
         zz=z*z;
			z = cos(ang)*sqrt(xx+zz);
			x = sin(ang)*sqrt(xx+zz);
		break;
		case Z:
         xx=x*x;
         yy=y*y;
			x = cos(ang)*sqrt(yy+xx);
			y = sin(ang)*sqrt(yy+xx);
		break;
	}
}

void 	 Vector3 :: rotate(const double ang,Axis ax)
{
	double finalAng = DEG_TO_RAD(ang);;
   float xx,yy;
   float s = sin(finalAng);
   float c = cos(finalAng);

   switch(ax)
	{
		case X:
         yy = y*c - z*s;
         z = -y*s + z*c;
         y=yy;
		break;
		case Y:
         xx = x*c - z*s;
         z = -x*s + z*c;
         x = xx;
		break;
		case Z:
         xx = x*c - y*s;
         y = -x*s + y*c;
         x = xx;
		break;
	}
}

void 	 Vector3 :: rotateAxis(const double angle,Vector3 ax)
{
	Vector3 e(ax.unitary());
	double rad_angle = DEG_TO_RAD(angle);
//	double rad_angle = angle;
   float tmpX,tmpY,tmpZ;
   float c = cos(rad_angle);
   float s = sin(rad_angle);

	tmpX = x*(e.x*e.x + (1-e.x*e.x)*c)
         +y*(e.x*e.y*(1-c) - e.z*s)
         +z*(e.x*e.z*(1-c) - e.y*s);

   tmpY = x*(e.x*e.y*(1-c) - e.z*s)
         +y*(e.y*e.y + (1-e.y*e.y)*c)
         +z*(e.y*e.z*(1-c) - e.x*s);


   tmpZ = x*(e.x*e.z*(1-c) - e.y*s)
         +y*(e.y*e.z*(1-c) - e.x*s)
         +z*(e.z*e.z + (1-e.z*e.z)*c);

   x = tmpX;
   y = tmpY;
   z = tmpZ;
}



std::ostream &operator<<(std::ostream & os, const Vector3 & v)
{
   return os   << "(" << v.x <<", " << v.y <<", " << v.z << ")"<<std::endl;
}
