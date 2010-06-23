#ifndef _KERNEL_SHADE_H_
#define _KERNEL_SHADE_H_

#include "Kernels/KernelBase.h"
#include "GraphBasis/FrameBufferObject.h"

class KernelShade : public KernelBase {

public:
  KernelShade();

  KernelShade(int width, int height, GLuint positionTexId, GLuint normalTexId, GLuint diffuseTexId, GLuint specularTexId,GLuint lightTexId, int lightTexSize);
	~KernelShade();
  
  void renderShader(const GLfloat * lightModelViewMatrix);
  void step(const GLfloat * lightModelViewMatrix);
  void setActive( bool op , const GLfloat * lightModelViewMatrix);
  
private:
  GLuint m_lightModeleViewMatrixLoc;
};


#endif