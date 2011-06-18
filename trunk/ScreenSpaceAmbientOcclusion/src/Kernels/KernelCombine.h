/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Apr 2011
 *
 *  A Kernel that combines two textures using a shader.
 *  The Color are blended with multiplication
 */

#ifndef _KERNEL_COMBINE_H_
#define _KERNEL_COMBINE_H_

#include "Kernels/KernelBase.h"

class KernelCombine : public KernelBase {

public:
  /**
  * Construct the kernel, create the shader and set the first texture
  */
  KernelCombine(int width, int height, 
                GLuint colorTexId);

  /**
   * Destory the kernel
   */
	~KernelCombine();
 
   /**
   * Gets the combined texture
   */
  GLuint getColorTexId() const;

  /**
   * Does the rendering cycle.
   *  The texture passed id the texture to be combined
   */
  void step(GLuint ssaoTexId);

private:
  /**
   * Color texture ID
   */
  GLuint m_colorTexId;
};


#endif