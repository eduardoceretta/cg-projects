/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
                  Matricula:2610166
\**********************************************************/
/**********************************************************\
		Cabeçaho Color
   Contem a definicao dasclasse Color
\**********************************************************/

#ifndef _COLOR_H_
#define _COLOR_H_

#include <iostream>

#include "GraphBasisDefines.h"

class Color
{
public:
   float r,g,b;
   Color(float R=0.0,float G=0.0,float B=0.0):r(R),g(G),b(B){}
   void setColor() const;

   void operator+= (const Color& v);
   void operator-= (const Color& v);
   void operator*= (const int val);
   void operator*= (const float val);
   void operator*= (const double val);


   Color operator* (const int val) const;
   Color operator* (const float val) const;
   Color operator* (const double val) const;
   Color operator+(const Color& v) const;
   Color operator-(const Color& v) const;
};

std::ostream &operator<<(std::ostream & os, const Color & c);
#endif
