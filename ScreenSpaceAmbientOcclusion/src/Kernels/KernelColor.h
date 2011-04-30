#ifndef _KERNEL_COLOR_H_
#define _KERNEL_COLOR_H_

#include "Kernels/KernelBase.h"
#include "GraphBasis/FrameBufferObject.h"

class KernelColor : public KernelBase {

public:
  enum textures {Position = 0, Normal, Diffuse, Specular};
  KernelColor();

  KernelColor(int width, int height);
	~KernelColor();

  GLuint getTexIdColor() const;
  
private:
  GLuint m_texIdColor;
};


#endif