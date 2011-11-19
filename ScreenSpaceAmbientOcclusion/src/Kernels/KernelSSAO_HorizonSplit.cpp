/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Jun 2011
 *
 *  Perform the calculation of the Screen Space Ambient Occlusion.
 *  Receive projection matrix information and reprojects the pixels obtained from the depth
 *  input textures and does visibility tests with its surrounds
 */

#include "KernelSSAO_HorizonSplit.h"



KernelSSAO_HorizonSplit::KernelSSAO_HorizonSplit(char* path, int width, int height, 
                       GLuint depht0_normalTexId)
: KernelBase(path, "ssao_vis.vert", "ssao_vis.frag", width, height){
	//Output
  m_colorTexId = addOutput(0);

	//Input
	m_shader->setActive(true);
    addInputTexture(GL_TEXTURE_2D, "depth0_normalTex", depht0_normalTexId);

    addInputFloat("screenWidth", width);
    addInputFloat("screenHeight", height);
	m_shader->setActive(false);
}

KernelSSAO_HorizonSplit::~KernelSSAO_HorizonSplit(){

}

GLuint KernelSSAO_HorizonSplit::getColorTexId() const
{
  return m_colorTexId; 
}

void KernelSSAO_HorizonSplit::step(float* projectionMatrix, float rfar, float pixelmask_size, GLfloat offsets_size, GLfloat intensity)
{
  float x = projectionMatrix[0*4+0];
  float y = projectionMatrix[1*4+1];
  float z = projectionMatrix[2*4+2];
  float w = projectionMatrix[3*4+2];
  float z_near = w/(z - 1.0);
  float z_far = w * z_near/(w + 2.0 * z_near);
  float right = z_near/x;
  float top = z_near/y;

  m_shader->setActive(true);
    addInputFloat("rfar", rfar);
    addInputFloat("pixelmask", pixelmask_size);
    addInputFloat("near", z_near);
    addInputFloat("far", z_far);
    addInputFloat("right", right);
    addInputFloat("top", top);
    addInputFloat("offsets_size", offsets_size);
    addInputFloat("intensity", intensity);
  m_shader->setActive(false);

  KernelBase::step();
}

