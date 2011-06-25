/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Jun 2011  
 *
 *  Timer counter utility class
 */

#include <iostream>
#include "Objects\Timer.h"

Timer :: Timer()
:m_currT(clock())
,m_timeSinceLastCall(0.0)
{
}


void Timer :: reset()
{
  m_currT = clock();
  m_timeSinceLastCall = 0.0;
}
