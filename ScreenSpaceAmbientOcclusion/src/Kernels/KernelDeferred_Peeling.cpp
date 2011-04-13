#include "KernelDeferred_Peeling.h"

KernelDeferred_Peeling::KernelDeferred_Peeling()
{

}

KernelDeferred_Peeling::KernelDeferred_Peeling(int width, int height, GLuint dephtTexId)
: KernelBase("./resources/Shaders/deferred_peeling.vert", "./resources/Shaders/deferred_peeling.frag", width, height){
  m_fbo->attachToDepthBuffer(BufferType::RenderBufferObject);
  
  //Output
  m_texIdPosition.push_back(addOutput(0));
  m_texIdNormal.push_back(addOutput(1));
  //m_texIdDiffuse = addOutput(2);
  //m_texIdSpecular = addOutput(3);
	
  m_dummyTexId = dephtTexId;
	//Input
	m_shader->setActive(true);
    addInputTexture(GL_TEXTURE_2D, "dephtTex", m_dummyTexId);

    addInputFloat("screenWidth", width);
    addInputFloat("screenHeight", height);
	m_shader->setActive(false);
}

KernelDeferred_Peeling::~KernelDeferred_Peeling(){

}


GLuint KernelDeferred_Peeling::getTexIdPosition(int index) const
{
  return m_texIdPosition[index];
}

GLuint KernelDeferred_Peeling::getTexIdNormal(int index) const
{
  return m_texIdNormal[index];
}

//GLuint KernelDeferred_Peeling::getTexIdDiffuse() const
//{
//  return m_texIdDiffuse;
//}
//
//GLuint KernelDeferred_Peeling::getTexIdSpecular() const
//{
//  return m_texIdSpecular;
//}

void KernelDeferred_Peeling::step(int pass )
{
  if(pass > 0)
  {
    if(m_texIdPosition.size() - 1 < pass)
    {
      m_texIdPosition.push_back(addOutput(0));
      m_texIdNormal.push_back(addOutput(1));
    }

    //m_texIdDiffuse = addOutput(2);
    //m_texIdSpecular = addOutput(3);
    
    setOutput(0, m_texIdPosition[pass]);
    setOutput(1, m_texIdNormal[pass]);

    //Input
    m_shader->setActive(true);
      setInputTexture(GL_TEXTURE_2D, "dephtTex", m_texIdNormal[pass - 1], 0);
    m_shader->setActive(false);
  }else
  {
    setOutput(0, m_texIdPosition[0]);
    setOutput(1, m_texIdNormal[0]);
    m_shader->setActive(true);
    setInputTexture(GL_TEXTURE_2D, "dephtTex", m_dummyTexId, 0);
    m_shader->setActive(false);
  }
}
