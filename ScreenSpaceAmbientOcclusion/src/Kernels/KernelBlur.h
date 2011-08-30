/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Oct 2010  
 *
 *  A Kernel that receives a 2D texture as input, applies a blur and 
 *  produces a output texture.
 */
#ifndef _KERNEL_BLUR_H_
#define _KERNEL_BLUR_H_

#include "Kernels/KernelBase.h"

class KernelBlur : public KernelBase {

public:
  /**
   * Construct the Kernel and set the input texture ID
   */
  KernelBlur(char* path, int width, int height, GLuint inputTexId);

  /**
   * Destroy the kernel
   */
	~KernelBlur();
  
  /**
   * Set the input texture ID to be blurred
   */
  void setInputTexId(GLuint inputTexId);
 
  /**
   * Get the blurred output texture id
   */
  GLuint getBlurredTexId() const;
 
  /**
   * Performs the blurring process.
   *  pixelmask_size defines the size of the blurred area
   *  offsets_size is a value that adds an offset between each pixel read in the pixel texture
   *  intensity is multiplied by the final color of the pixel
   */
  void step(float pixelmask_size = 3.0, GLfloat offsets_size = 1.0, GLfloat intensity = 1.0);

private:
  /**
   * Output texture id
   */
  GLuint m_blurredTexId;
};


#endif