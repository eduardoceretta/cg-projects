/************************************************************************
  rcosta@tecgraf.puc-rio.br
  rodesp@tecgraf.puc-rio.br
  Apr 2011
*************************************************************************/

#ifndef _PARTICLES_VECTOR_RENDERER_H_
#define _PARTICLES_VECTOR_RENDERER_H_

#include "renderer.h"
#include "defines.h"
#include "positionsorter.h"

#include <shader/glslshader.h>

#include <stdio.h>

/************************************************************************
  Forward Declarations
*************************************************************************/

class PtcObstacle;
class UGLArray;
class UGLVertexArray;
class UGLTexture;
class PtcPositionSortedIndices;

/**
  Renders vector as lines.
*/
class PARTICLES_API PtcVectorRenderer
{
public:
  PtcVectorRenderer ();
  ~PtcVectorRenderer ();

  bool Init ();
  void Draw ();
  void SetCamera (const AlgVector & position, const AlgVector & direction);
  void SetModel (PtcPointSet* model);

  /**
   * Sets whether color transparency will be used for rendering. If not,
   * alpha component will be ignored.
   *
   * Default: false
   */
  void SetTransparencyEnabled (bool enabled);

private:
  void UpdateVectorIndices ();

private:
  PtcPositionSortedIndices* m_sorted_indexes;
  UGLArray* m_vector_indexes;
  PtcPointSet* m_model;
  bool m_transparency_enabled;
};

#endif