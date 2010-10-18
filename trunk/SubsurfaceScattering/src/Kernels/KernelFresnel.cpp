/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#include "Kernels/KernelFresnel.h"

KernelFresnel::KernelFresnel()
{

}

KernelFresnel::KernelFresnel(int width, int height, GLuint vertexInfoTexId, 
                             int numVertices, int sizeofVertexInfo,
                             float refractionIndex1, float refractionIndex2)
: KernelBase("./resources/Shaders/fresnel.vert", "./resources/Shaders/fresnel.frag", width, height){
  m_fbo->attachToDepthBuffer(BufferType::RenderBufferObject);
  
  //Output
  m_texIdFresnel = addOutput(0);
  //m_texIdVertexNeighbor = addOutput(1);
	

	//Input
	m_shader->setActive(true);
    addInputTexture(GL_TEXTURE_2D, "vertexInfoTex", vertexInfoTexId);
    //addInputTexture(GL_TEXTURE_2D, "rayDir", texIdRayDir);
    //addInputTexture(GL_TEXTURE_1D, "grid", texIdGrid);
    //addInputTexture(GL_TEXTURE_1D, "triangleList", texIdTriangleList);
    //addInputTexture(GL_TEXTURE_1D, "vertexes", texIdvertexes);

    addInputFloat("outWidth", width);
    addInputFloat("outHeight", height);
    addInputFloat("numVertices", numVertices);
    addInputFloat("sizeofVertexInfo", sizeofVertexInfo);
    addInputFloat("n1", refractionIndex1);
    addInputFloat("n2", refractionIndex2);

	m_shader->setActive(false);
}

KernelFresnel::~KernelFresnel(){

}


GLuint KernelFresnel::getTexIdFresnel() const
{
  return m_texIdFresnel;
}

GLuint KernelFresnel::getTexIdVertexNeighbor() const
{
  return m_texIdVertexNeighbor;
}

