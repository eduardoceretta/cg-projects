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

#define FRAMES 50

class Frames
{
  /**
   * Time instants
   */
  clock_t t1, t2;

  /**
   * Frame counter
   */
  long    cont_frames;

  /**
   * Frames per second variable
   */
  float   fps, fps_old;

  /**
   * Time to render last frame
   */
  float   timeFromLastFrame;

  /**
   * Indicates if the counter is paused
   */
  bool paused;
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
   * Calculate the Frame Rate and return it
   */
  float getFrames();

  /**
   *  Calculate the Time to render last frame and return it
   */
  inline float getTimeFromLastFrame();

  /**
   * Get the time to render last fratme
   */
  inline float accessTimeFromLastFrame();
  
  /**
   * Get number of frames
   */
  long getFrameNum();
};


inline float Frames :: getTimeFromLastFrame()
{
   if(paused)
      return 0.0;
   t2 = clock();
   timeFromLastFrame  = (double)(t2 - t1) / CLOCKS_PER_SEC;
   t1 = t2;

   cont_frames++;

   return timeFromLastFrame;
}

inline float Frames :: accessTimeFromLastFrame()
{
  return timeFromLastFrame;
}




#endif
