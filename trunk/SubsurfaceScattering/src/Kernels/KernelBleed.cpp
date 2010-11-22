#include "KernelBleed.h"

KernelBleed::KernelBleed(int width, int height, 
                         int numVertices, int sizeofVertexInfo,
                         GLuint vertexInfoTexId, 
                         GLuint neighborTexId, int neighborSize,
                         GLuint neighborRTexId, int neighborRSize,
                         GLuint fresnelTexId, int fresnelSize,
                         float refractionIndex1, float refractionIndex2)
: KernelBase("./resources/Shaders/bleed.vert", "./resources/Shaders/bleed.frag", width, height){
	//Output
  m_texIdColor = addOutput(0);

	//Input
	m_shader->setActive(true);
    addInputTexture(GL_TEXTURE_2D, "vertexInfoTex", vertexInfoTexId);
    addInputTexture(GL_TEXTURE_2D, "neighborTex", neighborTexId);
    addInputTexture(GL_TEXTURE_2D, "neighborRTex", neighborRTexId);
    addInputTexture(GL_TEXTURE_2D, "fresnelTex", fresnelTexId);

    addInputFloat("outWidth", width);
    addInputFloat("outHeight", height);
    addInputFloat("numVertices", numVertices);
    addInputFloat("sizeofVertexInfo", sizeofVertexInfo);
    addInputFloat("neighborSize", neighborSize);
    addInputFloat("neighborRSize", neighborRSize);
    addInputFloat("fresnelSize", fresnelSize);
    addInputFloat("n1", refractionIndex1);
    addInputFloat("n2", refractionIndex2);

	m_shader->setActive(false);
}

KernelBleed::~KernelBleed(){

}



GLuint KernelBleed::getTexIdColor() const
{
  return m_texIdColor; 
}