/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Apr 2011
 *
 *  A Kernel that gets the color buffer and puts into a texture.
 *  Do not executes any Shader
 */

#ifndef _KERNEL_COLOR_H_
#define _KERNEL_COLOR_H_

#include "Kernels/KernelBase.h"

class KernelColor : public KernelBase {
public:
  /**
   * Simple Constructor
   */
  KernelColor();

  /**
   * Creates a FBO and sets the output texture as the ColorAttachment0
   */
  KernelColor(int width, int height);

  /**
   * Destroy the Kernel
   */
	~KernelColor();

  /**
   * Gets the color texture
   */
  GLuint getTexIdColor() const;
  
private:

  /**
   * Color texture ID
   */
  GLuint m_texIdColor;
};


#endif