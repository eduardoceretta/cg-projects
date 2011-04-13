#ifndef _KERNEL_ANTIALIASING_N_H_
#define _KERNEL_ANTIALIASING_N_H_

#include "Kernels/KernelBase.h"
#include "GraphBasis/FrameBufferObject.h"

class KernelAntiAliasingN : public KernelBase {

public:
  KernelAntiAliasingN(int width, int height, GLuint colorTexId,GLuint factorTexId );
  ~KernelAntiAliasingN();

  void renderShader();
  void step(int i = 0);
  void setActive(bool op);

private:
  GLuint m_colorTexId;
  GLuint m_original_colorTexId;
};


#endif