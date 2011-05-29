#ifndef _KERNEL_COMBINE_H_
#define _KERNEL_COMBINE_H_

#include "Kernels/KernelBase.h"
#include "GraphBasis/FrameBufferObject.h"

class KernelCombine : public KernelBase {

public:

  KernelCombine(int width, int height, 
                GLuint colorTexId);
	~KernelCombine();
 
  GLuint getColorTexId() const;
  void step(GLuint ssaoTexId);

private:
  GLuint m_colorTexId;
};


#endif