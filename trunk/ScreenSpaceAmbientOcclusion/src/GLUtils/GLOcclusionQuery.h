/**
 *	Eduardo Ceretta Dalla Favera.
 *  eduardo.ceretta@gmail.com
 *
 *  Mar 2012  
 *
 *  OpenGL Occlusion Query utility class.
 */

#pragma once

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>

class GLOcclusionQuery
{
public:

  GLOcclusionQuery();
  ~GLOcclusionQuery();

  /**
   *	Clears the Occlusion Query. Needs a init call after.
   */
  void reset();
  
  /**
   *	Initializes The Occlusion Query 
   */
  void init();

  /**
   *	Starts The Occlusion Query
   */
  void begin();

  /**
   *	Stop Chronometer
   */
  void end();

  /**
   *	Busy wait for the returned OcclusionQuery.
   *  Returns number of fragments generated.
   */
  GLuint getNumberOfFragments();

protected:
  /**
   *	OcclusionQuery Holder
   */
  GLuint m_numFragments;

  /**
   *	GL Query ID
   */
  GLuint m_query;

  /**
   *	Is the result ready?
   */
  GLint m_available;
};




