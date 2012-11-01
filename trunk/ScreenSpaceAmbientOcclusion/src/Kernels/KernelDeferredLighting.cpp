/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Mar 2012
 *
 *  A Kernel that calculates pixel illumination from G-Buffers from deferred shading.
 *  Accepts a specific ambient component.
 */

#include "KernelDeferredLighting.h"

KernelDeferredLighting::KernelDeferredLighting(char* path, int width, int height, 
        GLuint normalTexId, GLuint positionTexId, GLuint ambientTexId, Vector3 lightPosition, Vector3 lightDiffColor)
: KernelBase(path, "deferredLighting.vert", "deferredLighting.frag", width, height){
	//Output
  m_shadedTexId = addOutput(0);

	//Input
	m_shader->setActive(true);
    addInputTexture(GL_TEXTURE_2D, "normalTex", normalTexId);
    addInputTexture(GL_TEXTURE_2D, "positionTex", positionTexId);
    addInputTexture(GL_TEXTURE_2D, "ambientTex", ambientTexId);
    addInputVec3("lightPosition", lightPosition);
    addInputVec3("lightDiffColor", lightDiffColor);
	m_shader->setActive(false);
}

KernelDeferredLighting::~KernelDeferredLighting()
{

}

GLuint KernelDeferredLighting::getShadedTexId() const
{
  return m_shadedTexId; 
}

void KernelDeferredLighting::setAmbientInputTexId(GLuint inputTexId)
{
  m_shader->setActive(true);
  setInputTexture(GL_TEXTURE_2D, "ambientTex", inputTexId, 2);
  m_shader->setActive(false);
}

void KernelDeferredLighting::step(bool ambientEnabled, bool diffuseEnabled, Vector3 ambColor)
{
  GLfloat p[16];
  glGetFloatv(GL_MODELVIEW_MATRIX, p);
  if(m_shader)
  {
    m_shader->setActive(true);
      m_shader->setUniformInt("ambientEnabled", ambientEnabled);
      m_shader->setUniformInt("diffuseEnabled", diffuseEnabled);
      m_shader->setUniformVec3("ambientColor", ambColor);

      m_shader->setUniformMatrix4("modelView", p);
      m_shader->setActive(false);
  }
  KernelBase::step();
}

void KernelDeferredLighting::stepShaderOnly(bool ambientEnabled, bool diffuseEnabled, Vector3 ambColor)
{
  GLfloat p[16];
  glGetFloatv(GL_MODELVIEW_MATRIX, p);
  if(m_shader)
  {
    m_shader->setActive(true);
      m_shader->setUniformInt("ambientEnabled", ambientEnabled);
      m_shader->setUniformInt("diffuseEnabled", diffuseEnabled);
      m_shader->setUniformVec3("ambientColor", ambColor);
      m_shader->setUniformMatrix4("modelView", p);
  }
    renderScreenQuad();
  if(m_shader)
    m_shader->setActive(false);
}