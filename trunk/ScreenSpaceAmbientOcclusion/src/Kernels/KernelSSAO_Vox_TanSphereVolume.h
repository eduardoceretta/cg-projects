/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Sep 2011
 *
 *  Calculate the AmbientOcclusion of a Scene using the voxelization of it.
 *  Verifies the number of full voxels inside the influence sphere.
 */
#ifndef _KERNEL_SSAO_VOXELIZATION_VOLUME_H_
#define _KERNEL_SSAO_VOXELIZATION_VOLUME_H_

#include "Kernels/KernelBase.h"
#include "GLUtils/GLTextureObject.h"
#include "GLUtils/GLProjectionMatrix.h"
#include <vector>

using namespace std;

class KernelSSAO_Vox_TanSphereVolume : public KernelBase {

public:
  /**
   * The render targets produced by the Kernel
   */
  enum textures {SSAO = 0, Debug};

  /**
   * Create the kernel.
   *  texIdEyePos holds the eye position and nearest eye of each fragment.
   *  texIdNormalDepth holds the normal and depth of each fragment.
   *  texIdVoxelGrid holds the 3D grid of the scene.
   *  texIdGridInvFunction holds the inverse function that convert the eye space normalized index to grid space
   */
  KernelSSAO_Vox_TanSphereVolume(char* path, int width, int height, GLuint texIdEyePos, GLuint texIdNormalDepth, 
                            GLuint texIdVoxelGrid, GLuint texIdGridInvFunction);
  /**
   * Destroy the kernel
   */
  ~KernelSSAO_Vox_TanSphereVolume();

   /**
   * Activate/Deactivate the Kernel's FBO and shader
   *  Calculate the projection parameters needed by the shader
   */
  void setActive(bool op, GLProjectionMatrix *projectionMatrix);

  /**
   * Activate/Deactivate the Kernel's shader
   *  Calculate the projection parameters needed by the shader
   */
  void setActiveShaderOnly(bool op, GLProjectionMatrix *projectionMatrix);

  /**
   * Does a kernel pass cycle.
   *  Activate the FBO and shader, bind the input texture, draw a screen quad
   *  and deactivate the shader and the FBO.
   *  Calculate the projection parameters needed by the shader
   */
  void step(GLProjectionMatrix *projectionMatrix, float rfarPercent, float contrast);

  /**
   * Get output SSAO texture Id.
   */
  GLuint getTexIdSSAO() const;
  
  
  /**
   * Render the specific sampler distribution directions.
   */
  void renderSamplerDistribution(int distribution);

private:

  void createSamplerTexture();

  void createBitCount16Texture();
 
  /**
   * Grid Dimensions	
   */
  int m_width;
  int m_height;

  /**
   * Texture ids of the render targets
   */
  GLuint m_texIdSSAO;
  GLuint m_texIdDebug;
  

  /**
   * Volume Samplers
   */
  int m_numSamplers;
  int m_numSamplersDistributions;
  int m_samplersWidth;
  GLuint m_texIdSamplers;


  /**
   * BitCount 16
   */
  int m_bitCount16Height;
  int m_bitCount16Width;
  GLuint m_texIdBitCount16;
};


#endif