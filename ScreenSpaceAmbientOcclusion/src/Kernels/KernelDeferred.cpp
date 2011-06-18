#include "KernelDeferred.h"

KernelDeferred::KernelDeferred()
{

}

KernelDeferred::KernelDeferred(int width, int height)
: KernelBase("./resources/Shaders/deferred.vert", "./resources/Shaders/deferred.frag", width, height){
  m_fbo->attachToDepthBuffer(GL_FBOBufferType::RenderBufferObject);
  
  //Output
  m_texIdPosition = addOutput(0);
  m_texIdNormal = addOutput(1);
  m_texIdDiffuse = addOutput(2);
  m_texIdSpecular = addOutput(3);
	

	//Input
	//m_shader->setActive(true);
    //addInputTexture(GL_TEXTURE_2D, "rayPos", texIdRayPos);
    //addInputTexture(GL_TEXTURE_2D, "rayDir", texIdRayDir);
    //addInputTexture(GL_TEXTURE_1D, "grid", texIdGrid);
    //addInputTexture(GL_TEXTURE_1D, "triangleList", texIdTriangleList);
    //addInputTexture(GL_TEXTURE_1D, "vertexes", texIdvertexes);

    //addInputFloat("gridSize", gridTexSize);
    //addInputFloat("triangleListSize", triangleListTexSize);
    //addInputFloat("vertexesSize", vertexesTexSize);
	//m_shader->setActive(false);
}

KernelDeferred::~KernelDeferred(){

}


GLuint KernelDeferred::getTexIdPosition() const
{
  return m_texIdPosition;
}

GLuint KernelDeferred::getTexIdNormal() const
{
  return m_texIdNormal;
}

GLuint KernelDeferred::getTexIdDiffuse() const
{
  return m_texIdDiffuse;
}

GLuint KernelDeferred::getTexIdSpecular() const
{
  return m_texIdSpecular;
}
