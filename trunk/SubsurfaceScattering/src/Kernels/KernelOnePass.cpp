#include "KernelOnePass.h"

KernelOnePass::KernelOnePass(int width, int height, 
                         int numVertices, int sizeofVertexInfo,
                         GLuint vertexInfoTexId, 
                         GLuint qTexId, int qSize,
                         int sphereDivisionsTeta, int sphereDivisionsAlpha,
                         float refractionIndex1, float refractionIndex2)
: KernelBase("./resources/Shaders/onePass.vert", "./resources/Shaders/onePass.frag", width, height){
	//Output
  m_texIdColor = addOutput(0);

	//Input
	m_shader->setActive(true);
    addInputTexture(GL_TEXTURE_2D, "vertexInfoTex", vertexInfoTexId);
    addInputTexture(GL_TEXTURE_2D, "qTex", qTexId);

    addInputFloat("outWidth", width);
    addInputFloat("outHeight", height);
    addInputFloat("numVertices", numVertices);
    addInputFloat("sizeofVertexInfo", sizeofVertexInfo);
    addInputFloat("qSize", qSize);
    addInputFloat("sphereDivisionsTeta", sphereDivisionsTeta);
    addInputFloat("sphereDivisionsAlpha", sphereDivisionsAlpha);
    addInputFloat("n1", refractionIndex1);
    addInputFloat("n2", refractionIndex2);

	m_shader->setActive(false);
}

KernelOnePass::~KernelOnePass(){

}



GLuint KernelOnePass::getTexIdColor() const
{
  return m_texIdColor; 
}