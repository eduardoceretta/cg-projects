/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#ifndef _KERNEL_FRESNEL_H_
#define _KERNEL_FRESNEL_H_

#include "Kernels/KernelBase.h"
#include "GraphBasis/FrameBufferObject.h"

#include "main.h"

class KernelFresnel : public KernelBase {

public:
  enum textures {Fresnel = 0, VertexNeighbor};
  KernelFresnel();

  KernelFresnel(int width, int height, GLuint vertexInfoTexId, int numVertices, int sizeofVertexInfo, float refractionIndex1 = REFRACTIVE_INDEX_N1, float refractionIndex2 = REFRACTIVE_INDEX_N2);
	~KernelFresnel();

  GLuint getTexIdFresnel() const;
  GLuint getTexIdVertexNeighbor() const;

private:
  GLuint m_texIdFresnel;
  GLuint m_texIdVertexNeighbor;
};


#endif