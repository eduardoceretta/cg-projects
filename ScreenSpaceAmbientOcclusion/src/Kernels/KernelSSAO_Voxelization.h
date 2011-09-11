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

class KernelSSAO_Voxelization : public KernelBase {

public:
  /**
   * The render targets produced by the Kernel
   */
  enum textures {SSAAO = 0, Color, Grid0};

  /**
   * Create the kernel, the textures of the grid and the model information.
   *  Size is the grid dimensions number of textures (depends of the number of render targets)
   */
  KernelSSAO_Voxelization(char* path, int width, int height, GLuint texIdNormal, GLuint texIdVoxelGrid);
	
  /**
   * Destroy the kernel
   */
  ~KernelSSAO_Voxelization();


  ///**
  // * Activate/Deactivate the Kernel's FBO and shader
  // *  Calculate the projection parameters needed by the shader
  // */
  //void setActive(bool op);

  ///**
  // * Activate/Deactivate the Kernel's shader
  // *  Calculate the projection parameters needed by the shader
  // */
  //void setActiveShaderOnly(bool op);
  
  ///**
  // * Changes the Grid Function
  // */
  //void reloadGridFuncTextures(float power);
private:
  ///**
  // * Create the grid function textures[Normal and Inverse].
  // *  Maps the normalized distance to the respective normalized index of the grid.
  // *  Power is a function modificator
  // */
  void createRayDirectionsTexture();


  ///**
  // * Calculate the center and size of the specific Grid Cell
  // */
  //Vector3 getGridCellCenter(int x, int y, int z, float zNear);
  //Vector3 getGridCellSize(int x, int y, int z, float zNear);
  /**
   * Grid Dimensions	
   */
  int m_width;
  int m_height;

  /**
   * Texture ids of the render targets
   */
  GLuint m_texIdSSAO;

  
  /**
   * Input textures ids of the grid information
   */
  GLuint m_texIdRayDirections;
  int m_rayDirectionsWidth;
  int m_numRayHemispherDivision;
  int m_numRayDirections;
  int m_numRayDistribution;
};


#endif