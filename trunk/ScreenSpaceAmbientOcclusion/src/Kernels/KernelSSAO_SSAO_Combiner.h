/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Jan 2013
 *
 *  Combines a previous calculated ao with a screen space ao
 */

#ifndef _KERNEL_SSAO_COMBINER_H_
#define _KERNEL_SSAO_COMBINER_H_

#include "Kernels/KernelBase.h"

class GLProjectionMatrix;

class KernelSSAO_SSAO_Combiner : public KernelBase {

public:
  /**
   * Construct the kernel. 
   *  Receive a depth texId textures. 
   */
  KernelSSAO_SSAO_Combiner(char* path, int width, int height, 
              GLuint depth_normalTexId, GLuint ao_to_combineTexId);
	~KernelSSAO_SSAO_Combiner();
 
  /**
   * Return the result ID
   */
  GLuint getColorTexId() const;
 
  /**
   * Do a rendering cycle.
   *  projectionMatrix are OpenGL projection matrix respective to the scene drawn
   *  rfar is a shader parameter that controls the influence of the neighborhood occlusion
   *  offset_size controls the search area of the shader in screen space
   *  intensity affects directly the final result
   */
  void step(GLProjectionMatrix* projectionMatrix, float rfar, GLfloat offsets_size, GLfloat intensity);

private:
  /**
   * Output texture ID
   */
  GLuint m_colorTexId;

  /**
   * Create the texture that is used to access the neighborhood of a pixel
   */
  GLuint createSampleTexture();
};


#endif