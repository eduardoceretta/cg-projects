#ifndef _KERNEL_BLEED_H_
#define _KERNEL_BLEED_H_

#include "Kernels/KernelBase.h"
#include "GraphBasis/FrameBufferObject.h"

#include "main.h"

class KernelBleed : public KernelBase {

public:
  enum textures {Bleed = 0};
  KernelBleed(int width, int height, 
              int numVertices, int sizeofVertexInfo,
              GLuint vertexInfoTexId, 
              GLuint neighborTexId, int neighborSize,
              GLuint neighborRTexId, int neighborRSize,
              GLuint fresnelTexId, int fresnelSize,
              float refractionIndex1 = REFRACTIVE_INDEX_N1, float refractionIndex2 = REFRACTIVE_INDEX_N2);
	~KernelBleed();
  
  GLuint getTexIdColor() const;

  
private:
  GLuint m_texIdColor;
};


#endif