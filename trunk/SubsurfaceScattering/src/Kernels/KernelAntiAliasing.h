#ifndef _KERNEL_ANTIALIASING_H_
#define _KERNEL_ANTIALIASING_H_

#include "Kernels/KernelBase.h"
#include "GraphBasis/FrameBufferObject.h"

class KernelAntiAliasing : public KernelBase {

public:
  KernelAntiAliasing(int width, int height, GLuint normalTexId);
  ~KernelAntiAliasing();

  void renderShader();
  void step();
  void setActive(bool op);
  
  GLuint getTexIdFactor() const;
private:
  GLuint m_factorTexId;
};


#endif