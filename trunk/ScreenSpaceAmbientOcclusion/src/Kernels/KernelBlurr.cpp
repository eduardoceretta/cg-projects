#include "KernelBlurr.h"



KernelBlurr::KernelBlurr(int width, int height, GLuint inputTexId)
: KernelBase("./resources/Shaders/blurr.vert", "./resources/Shaders/blurr.frag", width, height){
	//Output
  m_blurredTexId = addOutput(0);

	//Input
	m_shader->setActive(true);
    addInputTexture(GL_TEXTURE_2D, "inputTex", inputTexId);

    addInputFloat("screenWidth", width);
    addInputFloat("screenHeight", height);
	m_shader->setActive(false);
}

KernelBlurr::~KernelBlurr(){

}

GLuint KernelBlurr::getBlurredTexId() const
{
  return m_blurredTexId; 
}

void KernelBlurr::step(float pixelmask_size, GLfloat offsets_size, GLfloat intensity)
{
  m_shader->setActive(true);
    addInputFloat("pixelmask_size", pixelmask_size);
    addInputFloat("offsets_size", offsets_size);
    addInputFloat("intensity", intensity);
  m_shader->setActive(false);
  KernelBase::step();
}

//
//GLuint KernelBlurr::createSampleTexture()
//{
//  GLenum target = GL_TEXTURE_1D;
//  int mem_size = SAMPLER_VECTOR_SIZE / 4;
//
//  GLfloat pixels[] = SAMPLER_VECTOR_DATA;
//#endif
//
//  GLuint dummyTexId;
//  glGenTextures(1, &dummyTexId);
//  glBindTexture(target, dummyTexId);
//  glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//  glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//  glTexParameterf(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//  glTexParameterf(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//  glTexParameteri(target, GL_GENERATE_MIPMAP, GL_TRUE); 
//  glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F_ARB, mem_size, 0, GL_RGBA, GL_FLOAT, pixels);
//  glBindTexture(target, 0);
//  return dummyTexId;
//}