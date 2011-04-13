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

  KernelDeferred_Peeling(int width, int height, GLuint dephtTexId);
	~KernelDeferred_Peeling();

  void step(int pass);

  GLuint getTexIdPosition(int index) const;
  GLuint getTexIdNormal(int index) const;
  
  //GLuint getTexIdDiffuse() const;
  //GLuint getTexIdSpecular() const;
  
private:
  vector<GLuint> m_texIdPosition;
  vector<GLuint> m_texIdNormal;
  GLuint m_dummyTexId;
  //GLuint m_texIdDiffuse;
  //GLuint m_texIdSpecular;
};


#endif