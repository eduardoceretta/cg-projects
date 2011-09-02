/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Sep 2011
 *
 *  A Kernel that gets the depth and color buffer and puts into a texture.
 */

#include "KernelVoxDepth.h"

KernelVoxDepth::KernelVoxDepth()
{

}

KernelVoxDepth::KernelVoxDepth(char* path, int width, int height)
:KernelBase(path, "voxDepth.vert", "voxDepth.frag", width, height)
  m_fbo->attachToDepthBuffer(GL_FBOBufferType::RenderBufferObject);
  //Output
  m_texIdEyeNearest = addOutput(0);
  //m_texIdColor = addOutput(0);
}

KernelVoxDepth::~KernelVoxDepth(){

}


GLuint KernelVoxDepth::getTexIdColor() const
{
  return m_texIdColor;
}

GLuint KernelVoxDepth::getTexIdEyeNearest() const
{
  return m_texIdEyeNearest;
}
