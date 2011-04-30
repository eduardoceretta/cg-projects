#include "KernelColor.h"

KernelColor::KernelColor()
{

}

KernelColor::KernelColor(int width, int height)
: KernelBase(NULL, NULL, width, height){
  m_fbo->attachToDepthBuffer(BufferType::RenderBufferObject);
  
  //Output
  m_texIdColor = addOutput(0);
}

KernelColor::~KernelColor(){

}


GLuint KernelColor::getTexIdColor() const
{
  return m_texIdColor;
}
