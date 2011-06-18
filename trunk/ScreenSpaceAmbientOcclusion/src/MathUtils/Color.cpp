/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Jul 2008  
 *
 *  Color holder.
 */

#include "Color.h"
#include "GL/glut.h"

Color Color :: operator* (const int val) const
{
   Color aux(r*val,g*val, b*val);
   return aux;
}

Color Color :: operator* (const float val) const
{
   Color aux(r*val,g*val,b*val);
   return aux;
}

Color Color :: operator* (const double val) const
{
   Color aux(r*val,g*val,b*val);
   return aux;
}


void Color :: operator*= (const int val)
{
   r*=val; g*=val; b*=val;
}

void Color :: operator*= (const float val)
{
   r*=val; g*=val; b*=val;
}

void Color :: operator*= (const double val)
{
   r*=val; g*=val; b*=val;
}


void Color :: operator+= (const Color& v)
{
   r += v.r; g += v.g; b += v.b;
}

void Color :: operator -= (const Color& v)
{
   r-=v.r; g-=v.g; b-=v.b;
}


Color Color :: operator + (const Color& v) const
{
   Color aux( r + v.r, g + v.g, b + v.b );
   return( aux );
}

Color Color :: operator - (const Color& v) const
{
   Color aux( r - v.r, g - v.g, b - v.b );
   return( aux );
}


std::ostream &operator<<(std::ostream & os, const Color & c)
{
   return os   << "(" << c.r <<", " << c.g <<", " << c.b << ")"<<std::endl;
}


