/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Jun 2011  
 *
 *  Timer counter utility class
 */
#ifndef _TIMER_H_
#define _TIMER_H_

#include <stdio.h>
#include <time.h>


class Timer
{
  /**
   * Time instants
   */
  clock_t m_currT;

  /**
   * Time to render last frame
   */
  float m_timeSinceLastCall;

public:
  /**
   * Simple Constructor
   */
  Timer();

  /**
   * Reset the clock
   */
  void reset();

  /**
   *  Return the Time since last call in seconds.
   */
  inline double getTime();
  
  /**
   *  Return the Time since last call in milliseconds.
   */
  inline double getTimeInMilliseconds();
};


double Timer::getTime()
{
  clock_t t = clock();
  m_timeSinceLastCall = (double)(t - m_currT)/CLOCKS_PER_SEC;
  m_currT = t;
  return m_timeSinceLastCall;
}

double Timer::getTimeInMilliseconds()
{
  clock_t t = clock();
  m_timeSinceLastCall = (double)(t - m_currT)/CLOCKS_PER_SEC;
  m_currT = t;
  return m_timeSinceLastCall*1e3;
}

#endif
