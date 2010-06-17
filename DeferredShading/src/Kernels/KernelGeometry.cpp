#include "KernelGeometry.h"

KernelGeometry::KernelGeometry()
{

}

KernelGeometry::KernelGeometry(int width, int height)
: KernelBase("./resources/Shaders/geometry.vert", "./resources/Shaders/geometry.frag", width, height){
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

KernelGeometry::~KernelGeometry(){

}


GLuint KernelGeometry::getTexIdPosition() const
{
  return m_texIdPosition;
}

GLuint KernelGeometry::getTexIdNormal() const
{
  return m_texIdNormal;
}

GLuint KernelGeometry::getTexIdDiffuse() const
{
  return m_texIdDiffuse;
}

GLuint KernelGeometry::getTexIdSpecular() const
{
  return m_texIdSpecular;
}
