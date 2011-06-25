/**
 *	Eduardo Ceretta Dalla Favera with modifications of Cesar Tadeu Pozzer
 *  eduardo.ceretta@gmail.com, pozzer3@gmail.com
 *  Mar 2010  
 *
 *  Frames control utility class
 */
#ifndef _FRAMES_H_
#define _FRAMES_H_

#include <stdio.h>
#include <time.h>

/**
 * Time in seconds to update the value of FPS
 */
#define UPDATE_TIME .5f

class Frames
{
  /**
   * Time instants
   */
  clock_t m_currT;

  /**
   * Frame counter
   */
  long m_numFrames;

  /**
   * Update Time counter. When is equal to UPDATE_TIME the value of 
   *  m_fps will be updated.
   */
  double m_updateTime;

  /**
   * Frames per second variable
   */
  float m_fps;

  /**
   * Time to render last frame
   */
  float m_timeSinceLastFrame;

  /**
   * Indicates if the counter is paused
   */
  bool m_paused;
public:
  /**
   * Simple Constructor
   */
  Frames();


  /**
   * Pause the clock
   */
  void pauseClock();

  /**
   * Reset the clock
   */
  void resetClock();

  /**
   * Update the clock. Must be called once per frame.
   */
  inline void update();

  /**
   * Return the Frames Per second
   */
  float getFPS();

  /**
   *  Return the Time since last frame.
   */
  float getTimeSinceLastFrame();
};


void Frames::update()
{
  if(m_paused)
    return;

  clock_t t = clock();
  m_timeSinceLastFrame = (double)(t - m_currT)/CLOCKS_PER_SEC;
  m_currT = t;

  m_numFrames++;
  m_updateTime += m_timeSinceLastFrame;

  if(m_updateTime >= UPDATE_TIME)
  {
    m_fps = (double)m_numFrames / m_updateTime;
    m_updateTime = 0.0;
    m_numFrames = 0;
  }
}

#endif
