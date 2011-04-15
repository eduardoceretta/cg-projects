#ifndef _KERNEL_DEFERRED_PEELING_H_
#define _KERNEL_DEFERRED_PEELING_H_

#include "Kernels/KernelBase.h"
#include "GraphBasis/FrameBufferObject.h"
#include <vector>

using namespace std;

class KernelDeferred_Peeling : public KernelBase {

public:
  enum textures {Position = 0, Normal, Diffuse, Specular};
  KernelDeferred_Peeling();

  KernelDeferred_Peeling(int width, int height, GLuint dephtTexId, int numPeelings = 1);
	~KernelDeferred_Peeling();

  void step(int pass);

  GLuint getTexIdPosition(int index) const;
  GLuint getTexIdNormal(int index) const;
  
  GLuint getTexIdDiffuse(int index) const;
  GLuint getTexIdSpecular(int index) const;
  
private:
  vector<GLuint> m_texIdPosition;
  vector<GLuint> m_texIdNormal;
  vector<GLuint> m_texIdDiffuse;
  vector<GLuint> m_texIdSpecular;
  GLuint m_dummyTexId;
};


#endif