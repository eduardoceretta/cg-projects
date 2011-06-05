#include "KernelCombine.h"



KernelCombine::KernelCombine(int width, int height, 
                             GLuint colorTexId)
: KernelBase("./resources/Shaders/combine.vert", "./resources/Shaders/combine.frag", width, height){
	//Output
  m_colorTexId = addOutput(0);

	//Input
	m_shader->setActive(true);

    addInputTexture(GL_TEXTURE_2D, "colorTex", colorTexId);
    addInputTexture(GL_TEXTURE_2D, "ssaoTex", colorTexId);

    //addInputFloat("screenWidth", width);
    //addInputFloat("screenHeight", height);
	m_shader->setActive(false);
}

KernelCombine::~KernelCombine(){

}

GLuint KernelCombine::getColorTexId() const
{
  return m_colorTexId; 
}

void KernelCombine::step(GLuint ssaoTexId)
{
  m_shader->setActive(true);
    setInputTexture(GL_TEXTURE_2D, "ssaoTex", ssaoTexId, 1);
  m_shader->setActive(false);
  KernelBase::step();
}
