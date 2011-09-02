/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Sep 2011
 *
 *  A Kernel that gets the depth and color buffer and puts into a texture.
 *  Get the color and the depth. The depth is the eye distance from the nearest fragment.
 */

#ifndef _KERNEL_VOX_DEPTH_H_
#define _KERNEL_VOX_DEPTH_H_

#include "Kernels/KernelBase.h"

class KernelVoxDepth : public KernelBase {
public:
  /**
   * Simple Constructor
   */
  KernelVoxDepth();

  /**
   * Creates the kernel FBO and output textures
   */
  KernelVoxDepth(char* path, int width, int height);

  /**
   * Destroy the Kernel
   */
	~KernelVoxDepth();

  /**
   * Gets the color texture
   */
  GLuint getTexIdColor() const;

  /**
   * Gets the eye nearest texture
   */
  GLuint getTexIdEyeNearest() const;
  
private:

  /**
   * Color texture ID
   */
  GLuint m_texIdColor;

  /**
   * Eye Nearest texture ID
   */
  GLuint m_texIdEyeNearest;
};


#endif