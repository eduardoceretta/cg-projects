/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
                  Matricula:2610166
\**********************************************************/
/**********************************************************\
                 Arquivo Color
   Contem a implementaçao dos metodos da classeColor
\**********************************************************/

#include "Color.h"
#include "GL/glut.h"

////////////////////
//~ Color
///////////////////
void Color :: setColor() const
{
   glColor3f(r,g,b);
}


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


