/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#ifndef _KERNEL_FRESNEL_H_
#define _KERNEL_FRESNEL_H_

#include "Kernels/KernelBase.h"
#include "GraphBasis/FrameBufferObject.h"

class KernelFresnel : public KernelBase {

public:
  enum textures {VertexInfo = 0, VertexNeighbor};
  KernelFresnel();

  KernelFresnel(int width, int height);
	~KernelFresnel();

  GLuint getTexIdVertexInfo() const;
  GLuint getTexIdVertexNeighbor() const;
  
private:
  GLuint m_texIdVertexInfo;
  GLuint m_texIdVertexNeighbor;
};


#endif