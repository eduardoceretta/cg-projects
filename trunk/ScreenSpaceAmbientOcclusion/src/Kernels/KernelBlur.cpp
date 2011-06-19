/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Apr 2011  
 *
 *  A Kernel that receives a 2D texture as input, applies a blur and 
 *  produces a output texture.
 */
#include "KernelBlur.h"


KernelBlur::KernelBlur(int width, int height, GLuint inputTexId)
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

KernelBlur::~KernelBlur(){

}

GLuint KernelBlur::getBlurredTexId() const
{
  return m_blurredTexId; 
}

void KernelBlur::step(float pixelmask_size, GLfloat offsets_size, GLfloat intensity)
{
  m_shader->setActive(true);
    addInputFloat("pixelmask_size", pixelmask_size);
    addInputFloat("offsets_size", offsets_size);
    addInputFloat("intensity", intensity);
  m_shader->setActive(false);
  KernelBase::step();
}
