/**
 *	Eduardo Ceretta Dalla Favera from Fabio Markus Miranda.
 *  eduardo.ceretta@gmail.com
 *
 *  Feb 2012  
 *
 *  OpenGL Timer utility class.
 */

#pragma once

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>

class GLTimer
{
public:

  GLTimer();
  ~GLTimer();

  /**
   *	Clears the chronometer. Needs a init call after.
   */
  void reset();
  
  /**
   *	Initializes Time Chronometer 
   */
  void init();

  /**
   *	Start Chronometer
   */
  void start();

  /**
   *	Stop Chronometer
   */
  void stop();

  /**
   *	Busy wait for the returned time.
   *  Returns time in nanoseconds
   */
  GLuint64EXT getElapsedTime();

  /**
   *	Busy wait for the returned time.
   *  Returns time in milliseconds
   */
  double getElapsedTimeInMilliseconds();

protected:
  /**
   *	Time Holder
   */
  GLuint64EXT m_timeElapsed;

  /**
   *	GL Query ID
   */
  GLuint m_query;

  /**
   *	Is the result ready?
   */
  GLint m_available;
};




