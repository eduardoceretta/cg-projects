/**
 *	Eduardo Ceretta Dalla Favera with modifications of Cesar Tadeu Pozzer
 *  eduardo.ceretta@gmail.com, pozzer3@gmail.com
 *  Mar 2010  
 *
 *  Frames control utility class
 */

#include <iostream>
#include <string.h>
#include "Objects\Frames.h"

Frames :: Frames()
:m_currT(clock())
,m_numFrames(0)
,m_updateTime(0.0)
,m_timeSinceLastFrame(0.0)
,m_fps(0.0)
,m_paused(false)
{
}

float Frames :: getFPS()
{
  return m_fps;
}

void Frames :: resetClock()
{
  m_currT = clock();
  m_numFrames = 0;
  m_updateTime = 0.0;
  m_timeSinceLastFrame = 0.0;
  m_fps = 0.0;
  m_paused = false;
}

void Frames :: pauseClock()
{
   m_paused = true;
}

float Frames::getTimeSinceLastFrame()
{
  return m_timeSinceLastFrame;
}
