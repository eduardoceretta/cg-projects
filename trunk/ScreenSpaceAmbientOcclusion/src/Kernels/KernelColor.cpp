/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Apr 2011
 *
 *  A Kernel that gets the color buffer and puts into a texture.
 *  Do not executes any Shader
 */

#include "KernelColor.h"

KernelColor::KernelColor()
{

}

KernelColor::KernelColor(int width, int height)
: KernelBase(NULL, NULL, width, height){
  m_fbo->attachToDepthBuffer(GL_FBOBufferType::RenderBufferObject);
  //Output
  m_texIdColor = addOutput(0);
}

KernelColor::~KernelColor(){

}


GLuint KernelColor::getTexIdColor() const
{
  return m_texIdColor;
}
