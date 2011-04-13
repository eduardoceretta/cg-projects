#ifndef _KERNEL_DEFERRED_H_
#define _KERNEL_DEFERRED_H_

#include "Kernels/KernelBase.h"
#include "GraphBasis/FrameBufferObject.h"

class KernelDeferred : public KernelBase {

public:
  enum textures {Position = 0, Normal, Diffuse, Specular};
  KernelDeferred();

  KernelDeferred(int width, int height);
	~KernelDeferred();

  GLuint getTexIdPosition() const;
  GLuint getTexIdNormal() const;
  GLuint getTexIdDiffuse() const;
  GLuint getTexIdSpecular() const;
  
private:
  GLuint m_texIdPosition;
  GLuint m_texIdNormal;
  GLuint m_texIdDiffuse;
  GLuint m_texIdSpecular;
};


#endif