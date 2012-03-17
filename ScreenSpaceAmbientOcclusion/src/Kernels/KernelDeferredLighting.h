/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Mar 2012
 *
 *  A Kernel that calculates pixel illumination from G-Buffers from deferred shading.
 *  Accepts a specific ambient component.
 */

#ifndef _KERNEL_DEFERRED_LIGHTING_H_
#define _KERNEL_DEFERRED_LIGHTING_H_

#include "Kernels/KernelBase.h"

class KernelDeferredLighting : public KernelBase {

public:
  /**
  * Construct the kernel, create the shader and set the textures
  */
  KernelDeferredLighting(char* path, int width, int height, 
                GLuint normalTexId, GLuint positionTexId, GLuint ambientTexId,Vector3 lightPosition, Vector3 lightDiffColor);

  /**
   * Destroy the kernel
   */
	~KernelDeferredLighting();
 
   /**
   * Gets the shaded texture
   */
  GLuint getShadedTexId() const;

  /**
   *	Executes the kernel.
   *    AmbEnabled determines if the ambient information will be used.
   *    AmbEnabled determines if the diffuse light will be calculated.
   *    AmbColor determines the ambient light color. Does not consider ambient material.
   */
  virtual void step(bool ambientEnabled, bool diffuseEnabled, Vector3 ambColor);

  /**
   *	Executes the kernel directs into the current frame buffer.
   *    AmbEnabled determines if the ambient information will be used.
   *    AmbEnabled determines if the diffuse light will be calculated.
   *    AmbColor determines the ambient light color. Does not consider ambient material.
   */
  void stepShaderOnly(bool ambientEnabled, bool diffuseEnabled, Vector3 ambColor);

private:
  /**
   * Color texture ID
   */
  GLuint m_shadedTexId;
};


#endif