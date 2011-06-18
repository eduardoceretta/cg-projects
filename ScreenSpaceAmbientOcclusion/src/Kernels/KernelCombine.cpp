/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Apr 2011
 *
 *  A Kernel that combines two textures using a shader.
 *  The Color are blended with multiplication
 */
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
