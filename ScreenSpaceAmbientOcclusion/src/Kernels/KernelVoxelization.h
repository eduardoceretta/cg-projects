/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Aug 2011
 *
 *  Voxelizes the scene in one pass using the gpu.
 *  Create a non regular grid and voxelizes the scene into it. Uses blending to make a bitmask that defines the grid.
 *  Based on: Elmar Eisemann, Xavier Decoret - Single-Pass GPU Solid Voxelization for Real-Time Applications - 2008
 */
#ifndef _KERNEL_VOXELIZATION_H_
#define _KERNEL_VOXELIZATION_H_

#include "Kernels/KernelBase.h"
#include <vector>

using namespace std;

class KernelVoxelization : public KernelBase {

public:
  /**
   * The render targets produced by the Kernel
   */
  enum textures {Normal = 0, Color, Grid0};

  /**
   * Create the kernel, the textures of the grid and the model information.
   *  Size is the grid dimensions number of textures (depends of the number of render targets)
   */
  KernelVoxelization(char* path, int width, int height, int size, GLuint gridFuncTexId);
	
  /**
   * Destroy the kernel
   */
  ~KernelVoxelization();

  /**
   * Get the id of the respective render target
   */
  GLuint getTexIdNormal(int index) const;
  GLuint getTexIdColor(int index) const;
  GLuint getTexIdGrid0(int index) const;

  
  /**
   * Activate/Deactivate the Kernel's FBO and shader
   *  Calculate the projection parameters needed by the shader
   */
  void setActive(bool op, GLfloat *projectionMatrix = NULL);

  /**
   * Activate/Deactivate the Kernel's shader
   *  Calculate the projection parameters needed by the shader
   */
  void setActiveShaderOnly(bool op, GLfloat *projectionMatrix = NULL);
  
  /**
   * Render the voxelization done by the algorithm.
   */
  void renderVoxelization(GLuint *data, Vector3 sizeBB, Vector3 centerBB, Vector3 camPos, Vector3 camAt, Vector3 camUp, GLfloat *projection, GLfloat* modelviewMatrix, float fov);
private:
  /**
   * Create the grid size texture.
   *  Maps the index of the grid to the correspondent bit mask.
   */
  void createGridBitMapTexture();

  /**
   * Create the grid function texture.
   *  Maps the normalized distance to the respective normalized index of the grid.
   */
  void createGridFuncTexture();

  /**
   *  Render the Camera Frustum	
   */
  void renderFrustum(GLfloat* projectionMatrix, GLfloat* modelviewMatrix);

  /**
   * Grid Dimensions	
   */
  int m_width;
  int m_height;
  int m_depth;

  /**
   * Texture ids of the render targets
   */
  GLuint m_texIdNormal;
  GLuint m_texIdColor;
  GLuint m_texIdGrid0;

  
  /**
   * Input textures ids of the grid information
   */
  GLuint m_texIdGridBitMap;
  int m_gridBitMapWidth;
  int m_gridBitMapHeight;

  GLuint m_texIdGridFunc;
  int m_gridFuncWidth;
};


#endif