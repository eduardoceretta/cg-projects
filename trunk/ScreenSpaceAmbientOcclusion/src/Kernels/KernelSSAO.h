#ifndef _KERNEL_SSAO_H_
#define _KERNEL_SSAO_H_

#include "Kernels/KernelBase.h"
#include "GraphBasis/FrameBufferObject.h"

class KernelSSAO : public KernelBase {

public:

  KernelSSAO(int width, int height, 
              GLuint diffuseColorTexId, GLuint specularColorTexId, 
              GLuint positionTexId, 
              GLuint depht0_normalTexId, GLuint depht1_normalTexId, 
              GLuint depht2_normalTexId);
	~KernelSSAO();
 
  GLuint getColorTexId() const;
 
  void step(float z_near, float z_far, float right, float top);
private:
  GLuint m_colorTexId;
};


#endif