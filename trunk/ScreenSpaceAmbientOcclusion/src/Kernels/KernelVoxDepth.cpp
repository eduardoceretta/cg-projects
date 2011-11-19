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
:KernelBase(path, "vox_depth.vert", "vox_depth.frag", width, height)
{
  m_fbo->attachToDepthBuffer(GL_FBOBufferType::RenderBufferObject);
  //Output
  m_texIdNormalDepth = addOutput(0);
  m_texIdEyePos = addOutput(1);
}

KernelVoxDepth::~KernelVoxDepth(){

}


GLuint KernelVoxDepth::getTexIdEyePos() const
{
  return m_texIdEyePos;
}

GLuint KernelVoxDepth::getTexIdNormalDepth() const
{
  return m_texIdNormalDepth;
}
