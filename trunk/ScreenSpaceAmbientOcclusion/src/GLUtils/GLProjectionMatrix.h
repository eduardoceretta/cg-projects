/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Aug 2011
 *
 *  OpenGL Projection Matrix extractor.
 *  Gets the Projection [Perspective|Orthographic] Matrix and its information
 */

#ifndef _GL_PROJECTION_MATRIX_H_
#define _GL_PROJECTION_MATRIX_H_

#include <GL/glut.h>
#include <string>

#include "assert.h"
#include "MathUtils/Matrix4.h"

using namespace std;

class GLProjectionMatrix: public Matrix4
{
public:
  /**
   * Simple Constructor. 
   */
  GLProjectionMatrix();

  /**
   * Constructor. 
   *  Receive a projection Matrix 4x4.
   */
  GLProjectionMatrix(float *m);
 

  /**
   * Frees memory.
   */
  ~GLProjectionMatrix();

  /**
   * Read OpenGL Projection Matrix
   */ 
  void readGLProjection();

  /**
   * Gets Projection Parameters
   */
  float getNear();
  float getFar();
  float getLeft();
  float getRight();
  float getBottom();
  float getTop();

  bool isPerspective();
  bool isOrthographic();

protected:
  void update();
  void updatePerspective();
  void updateOrthographic();
 
  typedef enum ProjectionType {Perspective = 0 , Orthographic = 1, None = 2};

  bool m_outdated;

  float m_top;
  float m_bottom;
  float m_right;
  float m_left;
  float m_near;
  float m_far;

  ProjectionType m_projType;
};

#endif


