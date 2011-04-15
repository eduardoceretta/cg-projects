#include "KernelSSAO.h"

KernelSSAO::KernelSSAO(int width, int height, 
                       GLuint diffuseColorTexId, GLuint specularColorTexId, 
                       GLuint positionTexId, 
                       GLuint depht0_normalTexId, GLuint depht1_normalTexId, 
                       GLuint depht2_normalTexId)
: KernelBase("./resources/Shaders/ssao.vert", "./resources/Shaders/ssao.frag", width, height){
	//Output
  m_colorTexId = addOutput(0);

  GLfloat f[16];
	//Input
	m_shader->setActive(true);
    addInputTexture(GL_TEXTURE_2D, "diffuseTex", diffuseColorTexId);
    addInputTexture(GL_TEXTURE_2D, "specularTex", specularColorTexId);
    addInputTexture(GL_TEXTURE_2D, "positionTex", positionTexId);
    addInputTexture(GL_TEXTURE_2D, "depth0_normalTex", depht0_normalTexId);
    addInputTexture(GL_TEXTURE_2D, "depth1_normalTex", depht1_normalTexId);
    addInputTexture(GL_TEXTURE_2D, "depth2_normalTex", depht2_normalTexId);

    addInputFloat("screenWidth", width);
    addInputFloat("screenHeight", height);
	m_shader->setActive(false);
}

KernelSSAO::~KernelSSAO(){

}

GLuint KernelSSAO::getColorTexId() const
{
  return m_colorTexId; 
}

void KernelSSAO::step(float z_near, float z_far, float right, float top)
{
  m_shader->setActive(true);
    addInputFloat("near", z_near);
    addInputFloat("far", z_far);
    addInputFloat("right", right);
    addInputFloat("top", top);
  m_shader->setActive(false);
  KernelBase::step();
}