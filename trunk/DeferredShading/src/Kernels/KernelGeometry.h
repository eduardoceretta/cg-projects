#ifndef _KERNEL_GEOMETRY_H_
#define _KERNEL_GEOMETRY_H_

#include "Kernels/KernelBase.h"
#include "SpacePartition/UniformGrid.h"
#include "GraphBasis/FrameBufferObject.h"

class KernelGeometry : public KernelBase {

public:
  enum textures {Position = 0, Normal, Diffuse, Specular};
  KernelGeometry();

  KernelGeometry(int width, int height);
	~KernelGeometry();

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