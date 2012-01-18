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


#define SPHERECENTER_PARM 2.5 // 3
#define SPHERERADIUS_PARM 0.5  // 1.5

#define PROG_A0 4
#define PROG_STEP 2

using namespace std;


//SPHERE INFO//////////////////////////////////////////
//SPHERE INFO//////////////////////////////////////////
//SPHERE INFO//////////////////////////////////////////
#define SPHERE_MAX_NUM 10
class SphereInfo
{
  float m_coneRevolutionAngle;
  int m_numSpheresByCone;
  int m_numCones;

public:
  enum CalcMethods{RadiusProgression = 0, RadiusInitDistance, EndEnum};
  CalcMethods currCalcMethod;

  struct RadDistParms{
    int numMaxSpheres;
    float xo;
    float sphereOverlap;
    RadDistParms()
      :numMaxSpheres(SPHERE_MAX_NUM)
      ,sphereOverlap(0.0f){}
  }radDistParms;

  struct RadProgParms
  {
    float sphereCenterParm; //SPHERECENTER_PARM 2.5 // 3
    float sphereRadiusParm;//SPHERERADIUS_PARM 0.5  // 1.5
    RadProgParms()
      :sphereCenterParm(3)
      ,sphereRadiusParm(1.5f){}
  }radProgParms;


  SphereInfo()
    :currCalcMethod(RadiusProgression)
  {}

  void setParameters(float coneRevolutionAngle, int numSpheresByCone, int numCones);

  void nextCalcMethod();

  void getSphereInfo(int k, float* center, float* radius);


private:
  float RadDist_GetRadius(float initDist, float coneRevolutionAngle);
  float RadDist_GetInitDist(float xo, int i, float coneRevolutionAngle);
  float RadDist_GetConeInitialDist(int numSpheresByCone, float coneRevolutionAngle);

  float RadProg_GetSphereCenterMultiplier(int i, int numSpheresByCone);
  float RadProg_GetSphereRadiusMultiplier(int i, int numSpheresByCone, int numCones, float coneRevolutionAngle);
};


//////KERNEL//////////////////////////////////////////////////////////////
//////KERNEL//////////////////////////////////////////////////////////////
//////KERNEL//////////////////////////////////////////////////////////////
//////KERNEL//////////////////////////////////////////////////////////////
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
  //void setNumSpheresByCone(int val);

  int getNumSamplersDistributions() const;
  void setNumSamplersDistributions(int val);

  float getConeRevolutionAngle() const;
  void setConeRevolutionAngle(float val);

  int getNumberSphereSamplers() const;
  void setNumSphereSamplers(int val);

  bool isJitterEnabled() const;
  void setJitterEnabled(bool val);

  float getRfarPercent() const;
  void setRfarPercent(float val);

  float getContrast() const;
  void setContrast(float val);


  SphereInfo* getSphereInfo();

  /**
   * Reload Shader Input
   */
  void reloadShaderInput();
  
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
   * Render the specific sampler distribution directions.
   */
  void renderConeDistribution(int distribution);
  void renderSphereSamplerDistribution(int distribution, int sphereIndex);
  void renderSphereInfoDistribution(int distribution);
private:


  
  /**
   * Create Data Textures 
   */
  void generateConeSamplerTexture();
  void generateSphereSamplerTexture();
  void generateSphereInfoTexture();
  void generateBitCount16Texture();
 
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
  float m_rfarPercent;
  float m_contrast;
  int m_numCones;
  int m_numSpheresByCone;
  int m_numSamplersDistributions;
  float m_coneRevolutionAngle;
  int m_numSphereSamplers;
  bool m_jitterEnabled;
  
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
  int m_sphereInfoWidth;
  GLuint m_texIdSphereInfo;

  int m_sphereSamplersWidth;     
  GLuint m_texIdSphereSamplers;  
  GLfloat* m_sphereSamplers;

  SphereInfo *m_sphereInfo;

  /**
   * Render Samplers Objects
   */
  GLPointLight m_pointLight;
  
};


#endif