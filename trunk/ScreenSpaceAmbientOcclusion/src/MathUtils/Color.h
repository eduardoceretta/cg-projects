/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Jul 2008  
 *
 *  Color holder.
 */

#ifndef _COLOR_H_
#define _COLOR_H_

#include <iostream>

class Color
{
public:
  /**
  * Color components
  */
  float r,g,b;

  /**
   * Simple Constructor
   */
  Color(){}
  Color(float R,float G,float B):r(R),g(G),b(B){}

  /**
   * Color operators.
   */
  void operator+= (const Color& v);
  void operator-= (const Color& v);
  void operator*= (const int val);
  void operator*= (const float val);
  void operator*= (const double val);

  Color operator+ (const Color& v) const;
  Color operator- (const Color& v) const;
  Color operator* (const int val) const;
  Color operator* (const float val) const;
  Color operator* (const double val) const;
};

/**
 * Stream output operator
 */
std::ostream &operator<<(std::ostream & os, const Color & c);
#endif
