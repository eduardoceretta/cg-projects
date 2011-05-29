#ifndef _KERNEL_BLURR_H_
#define _KERNEL_BLURR_H_

#include "Kernels/KernelBase.h"
#include "GraphBasis/FrameBufferObject.h"

class KernelBlurr : public KernelBase {

public:

  KernelBlurr(int width, int height, GLuint inputTexId);
	~KernelBlurr();
 
  GLuint getBlurredTexId() const;
 
  void step(float pixelmask_size = 3.0, GLfloat offsets_size = 1.0, GLfloat intensity = 1.0);
private:
  GLuint m_blurredTexId;
  GLuint createSampleTexture();

};


#endif