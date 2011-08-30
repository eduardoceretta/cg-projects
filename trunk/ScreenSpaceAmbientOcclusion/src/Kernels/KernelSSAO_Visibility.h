/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Jun 2011
 *
 *  Perform the calculation of the Screen Space Ambient Occlusion.
 *  Receive projection matrix information and reprojects the pixels obtained from the depth
 *  input textures and does visibility tests with its surrounds
 */
#ifndef _KERNEL_SSAO_VISIBILITY_H_
#define _KERNEL_SSAO_VISIBILITY_H_

#include "Kernels/KernelBase.h"

class KernelSSAO_Visibility : public KernelBase {

public:
  /**
   * Construct the kernel. 
   *  Receive 3 depth peelings textures. 
   */
  KernelSSAO_Visibility(char* path, int width, int height, 
              GLuint depht0_normalTexId);
	~KernelSSAO_Visibility();
 
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
  void step(float* projectionMatrix, float rfar, float pixelmask_size, GLfloat offsets_size, GLfloat intensity);

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