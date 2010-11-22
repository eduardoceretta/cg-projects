#ifndef _KERNEL_ONEPASS_H_
#define _KERNEL_ONEPASS_H_

#include "Kernels/KernelBase.h"
#include "GraphBasis/FrameBufferObject.h"

#include "main.h"

class KernelOnePass : public KernelBase {

public:
  enum textures {OnePass = 0};
  KernelOnePass(int width, int height, 
              int numVertices, int sizeofVertexInfo,
              GLuint vertexInfoTexId, 
              GLuint qTexId, int qSize,
              int sphereDivisionsTeta, int sphereDivisionsAlpha,
              float refractionIndex1 = REFRACTIVE_INDEX_N1, float refractionIndex2 = REFRACTIVE_INDEX_N2);
	~KernelOnePass();
  
  GLuint getTexIdColor() const;

  
private:
  GLuint m_texIdColor;
};


#endif