/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Sep 2011
 *
 *  Calculate the AmbientOcclusion of a Scene using the voxelization of it.
 *  Create some random ray distribution around the hemisphere and passes it as a texture.
 *  Access each ray in the voxel structure and verifies if it is empty, iterate each ray.
 */
#ifndef _KERNEL_SSAO_VOXELIZATION_H_
#define _KERNEL_SSAO_VOXELIZATION_H_

#include "Kernels/KernelBase.h"
#include "GLUtils/GLTextureObject.h"
#include "GLUtils/GLProjectionMatrix.h"
#include <vector>

using namespace std;

class KernelSSAO_Vox_RayMarch : public KernelBase {

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
  KernelSSAO_Vox_RayMarch(char* path, int width, int height, GLuint texIdEyePos, GLuint texIdNormalDepth, 
                            GLuint texIdVoxelGrid, GLuint texIdGridInvFunction);
  /**
   * Destroy the kernel
   */
  ~KernelSSAO_Vox_RayMarch();

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
  void step(GLProjectionMatrix *projectionMatrix);

  /**
   * Get output SSAO texture Id.
   */
  GLuint getTexIdSSAO() const;

  /**
   * Render the specific ray distribution directions.
   */
  void renderRayDistribution(int distribution);
private:
  /**
   * Create the ray directions to be used in the hemisphere traverse.
   *  Create n random variations of the hemisphere ray distribution.
   */
  void createRayDirectionsTexture();
 
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
   * Ray Directions Attributes
   */
  int m_numRayHemispherDivision;
  int m_numRayDirections;
  int m_numRaySteps;
  int m_numRayDistribution;

  /**
   * Input textures ids and information
   */
  GLuint m_texIdRayDirections;
  int m_rayDirectionsWidth;

};


#endif