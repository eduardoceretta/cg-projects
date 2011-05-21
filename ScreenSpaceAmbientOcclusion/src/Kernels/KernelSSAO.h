#ifndef _KERNEL_SSAO_H_
#define _KERNEL_SSAO_H_

#include "Kernels/KernelBase.h"
#include "GraphBasis/FrameBufferObject.h"

class KernelSSAO : public KernelBase {

public:

  KernelSSAO(int width, int height, 
              GLuint diffuseColorTexId, GLuint colorTexId, 
              GLuint positionTexId, 
              GLuint depht0_normalTexId, GLuint depht1_normalTexId, 
              GLuint depht2_normalTexId);
	~KernelSSAO();
 
  GLuint getColorTexId() const;
 
  void step(float z_near, float z_far, float right, float top, float rfar, float pixelmask_size, GLfloat offsets_size, GLfloat intensity);
private:
  GLuint m_colorTexId;
  GLuint createSampleTexture();
};


#endif