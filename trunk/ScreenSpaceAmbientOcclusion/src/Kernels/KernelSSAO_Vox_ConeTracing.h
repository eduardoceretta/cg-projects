/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Oct 2011
 *
 *  Calculate the AmbientOcclusion of a Scene using the voxelization of it.
 *  Make Cone Tracing to calculate the occlusion. Each Cone Integral is achieved 
 *  by calculating n sphere integrals.
 */
#ifndef _KERNEL_SSAO_VOXELIZATION_CONE_H_
#define _KERNEL_SSAO_VOXELIZATION_CONE_H_

#include "Kernels/KernelBase.h"
#include "GLUtils/GLTextureObject.h"
#include "GLUtils/GLProjectionMatrix.h"
#include "GLLights/GLPointLight.h"
#include <vector>

using namespace std;

class KernelSSAO_Vox_ConeTracing : public KernelBase {

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
  KernelSSAO_Vox_ConeTracing(char* path, int width, int height, GLuint texIdEyePos, GLuint texIdNormalDepth, 
                            GLuint texIdVoxelGrid, GLuint texIdGridInvFunction);
  /**
   * Destroy the kernel
   */
  ~KernelSSAO_Vox_ConeTracing();

  /**
   * Parameters Getters and Setters
   */ 
  int getNumCones() const;
  void setNumCones(int val);

  int getNumSpheresByCone() const;
  void setNumSpheresByCone(int val);

  int getNumSamplersDistributions() const;
  void setNumSamplersDistributions(int val);
  
  
  /**
   * Activate/Deactivate the Kernel's FBO and shader
   *  Calculate the projection parameters needed by the shader
   */
  void setActive(bool op, GLProjectionMatrix *projectionMatrix, float rfarPercent, float contrast);

  /**
   * Activate/Deactivate the Kernel's shader
   *  Calculate the projection parameters needed by the shader
   */
  void setActiveShaderOnly(bool op, GLProjectionMatrix *projectionMatrix, float rfarPercent, float contrast);

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
  void renderSphereSamplerDistribution(int distribution, int sphereIndex);
private:

  /**
   * Reload Shader Input
   */
  void reloadShaderInput();
  
  /**
   * Create Data Textures 
   */
  void setConeSamplerTexture();
  void setSphereSamplerTexture();
  void setBitCount16Texture();

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
   * Parameters
   */
  int m_numCones;
  int m_numSpheresByCone;
  int m_numSamplersDistributions;

  /**
   * Cone Dir Samplers
   */
  int m_coneDirSamplersWidth;
  GLuint m_texIdConeDirSamplers;


  /**
   * BitCount 16
   */
  int m_bitCount16Height;
  int m_bitCount16Width;
  GLuint m_texIdBitCount16;

  /**
   * Sphere Samplers
   */
  int m_sphereSamplersWidth;     
  GLuint m_texIdSphereSamplers;  
  GLfloat* m_sphereSamplers;

  /**
   * Render Samplers Objects
   */
  GLPointLight m_pointLight;
};


#endif