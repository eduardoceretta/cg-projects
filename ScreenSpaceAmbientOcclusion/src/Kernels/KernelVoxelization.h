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
#include "GLUtils/GLTextureObject.h"
#include "GLUtils/GLProjectionMatrix.h"
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
  KernelVoxelization(char* path, int width, int height, int size, GLuint texIdEyePos);
	
  /**
   * Destroy the kernel
   */
  ~KernelVoxelization();

  /**
   * Get the id of the respective render target
   */
  GLuint getTexIdNormal() const;
  GLuint getTexIdColor() const;
  GLuint getTexIdGrid0() const;

  /**
   * Get Voxelization Render Bounding Box
   */
  Vector3 getVoxBBMin();
  Vector3 getVoxBBMax();
  int getRenderMode() const;
  void setRenderMode(int val);

  /**
   * Grid Render Steps Gets and Sets
   */
  int getStepX() const;
  void setStepX(int val);

  int getStepY() const;
  void setStepY(int val);

  int getStepZ() const;
  void setStepZ(int val);

  /**
   * Grid Functions textures
   */
  GLuint getTexIdGridFunc() const;
  GLuint getTexIdGridInvFunc() const;

  /**
   * Activate/Deactivate the Kernel's FBO and shader
   *  Calculate the projection parameters needed by the shader
   */
  void setActive(bool op);

  /**
   * Activate/Deactivate the Kernel's shader
   *  Calculate the projection parameters needed by the shader
   */
  void setActiveShaderOnly(bool op);
  
  /**
   * Render the voxelization done by the algorithm.
   */
  void renderVoxelization();

  /**
   * Changes the Grid Function
   */
  void reloadGridFuncTextures(float power);
private:
  /**
   * Create the grid size texture.
   *  Maps the index of the grid to the correspondent bit mask.
   */
  void createGridBitMapTexture();

  /**
   * Create the grid function textures[Normal and Inverse].
   *  Maps the normalized distance to the respective normalized index of the grid.
   *  Power is a function modificator
   */
  void createGridFuncTextures(float power);

  /**
   *  Render the Camera Frustum	
   */
  void renderFrustum();

  /**
   * Calculate the BoundingBox
   */
  void updateBB();

  /**
   * Update Voxelization Texture and bring it to main memory
   */
  void updateData();

  /**
   * Calculate the center and size of the specific Grid Cell
   */
  Vector3 getGridCellCenter(int x, int y, int z, float zNear);
  Vector3 getGridCellSize(int x, int y, int z, float zNear);
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

  GLTextureObject m_texObj;
  GLTextureObject m_eyeNearestTexObj;
  GLTextureObject m_funcTexObj;
  GLfloat* m_eyeNearestData;
  GLfloat* m_funcData;
  GLuint m_funcTexSize;


  /**
   * Voxelization Render Parameters	
   */
  int m_renderMode;
  bool m_BBCalculated;
  float m_far;
  float m_top;
  float m_near;
  float m_right;
  Vector3 m_voxBBMin;
  Vector3 m_voxBBMax;
  GLuint* m_voxData;
  GLProjectionMatrix m_projectionMatrix;
  GLfloat m_modelviewMatrix[16];
  int m_stepX;
  int m_stepY;
  int m_stepZ;
  int m_initX;
  int m_initY;
  int m_initZ;
  int m_endX;
  int m_endY;
  int m_endZ;

 
  /**
   * Input textures ids of the grid information
   */
  GLuint m_texIdEyePos;

  GLuint m_texIdGridBitMap;
  int m_gridBitMapWidth;
  int m_gridBitMapHeight;

  GLuint m_texIdGridFunc;
  GLuint m_texIdGridInvFunc;
  
  int m_gridFuncWidth;

};


#endif