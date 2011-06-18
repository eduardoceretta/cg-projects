#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <assert.h>

/**
 * MyAssert
 *  If expression is false print expression and calls regular assert
 */
#ifdef NDEBUG
  #define MyAssert(str, expression)     ((void)0)  
#else
  #define MyAssert(str, expression) if(!(expression)){std::cout<<(str)<<std::endl;assert(expression);}
#endif

#endif