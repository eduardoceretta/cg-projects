/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Jul 2008  
 *
 *  Defines needed in the math classes
 */

#ifndef _MATH_DEF_H_
#define _MATH_DEF_H_

#define CIRCLE_ROUNDNESS 30

#define CHAR_W 10
#define CHAR_H 13

#define PI 3.1415926535897932384626433832795
#define RAD_TO_DEG(a) ((float)(a)*(180.0/PI))
#define DEG_TO_RAD(a) ((float)(a)*PI/180.0)

template <typename T>
inline T abs(T x)
{
   return x > 0 ? x : -x;
}

template <typename T>
inline T clamp(T v, T min, T max)
{
   if (v < min)
      return min;
   if (v > max)
      return max;
   return v;
}

#endif
