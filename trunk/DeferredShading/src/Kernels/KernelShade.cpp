#include "KernelShade.h"

KernelShade::KernelShade()
{

}

KernelShade::KernelShade(int width, int height, GLuint positionTexId, GLuint normalTexId, 
                         GLuint diffuseTexId, GLuint specularTexId, GLuint lightTexId, int lightTexSize)
: KernelBase("./resources/Shaders/shade.vert", "./resources/Shaders/shade.frag", width, height){
	//Output

  GLfloat f[16];
	//Input
	m_shader->setActive(true);
    addInputTexture(GL_TEXTURE_2D, "positionTex", positionTexId);
    addInputTexture(GL_TEXTURE_2D, "normalTex", normalTexId);
    addInputTexture(GL_TEXTURE_2D, "diffuseTex", diffuseTexId);
    addInputTexture(GL_TEXTURE_2D, "specularTex", specularTexId);
    addInputTexture(GL_TEXTURE_1D, "lightTex", lightTexId);

    addInputFloat("lightTexSize", lightTexSize);

    m_lightModeleViewMatrixLoc = addInputMatrix4("lightModelViewMatrix", f);
	m_shader->setActive(false);
}

KernelShade::~KernelShade(){

}


void KernelShade::renderShader(const GLfloat * lightModelViewMatrix)
{
  glPushAttrib(GL_ENABLE_BIT);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0, 1, 0, 1);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glEnable(GL_TEXTURE);
  glEnable(GL_TEXTURE_1D);
  glEnable(GL_TEXTURE_2D);

  m_shader->setActive(true);
  glUniformMatrix4fv(m_lightModeleViewMatrixLoc, 1, GL_FALSE, lightModelViewMatrix);
  
  activateTextures();
  glColor3f(1,1,1);
  glBegin(GL_QUADS);
  glTexCoord2f(0,0); glVertex3f(0,0,0);
  glTexCoord2f(1,0); glVertex3f(1,0,0);
  glTexCoord2f(1,1); glVertex3f(1,1,0);
  glTexCoord2f(0,1); glVertex3f(0,1,0);
  glEnd();
  m_shader->setActive(false);


  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glPopAttrib();
}

void KernelShade::step(const GLfloat * lightModelViewMatrix){
  m_fbo->setActive(true);
  m_shader->setActive(true);
  glUniformMatrix4fv(m_lightModeleViewMatrixLoc, 1, GL_FALSE, lightModelViewMatrix);
  activateTextures();
  renderQuad();
  m_shader->setActive(false);
  m_fbo->setActive(false);
}

void KernelShade::setActive( bool op , const GLfloat * lightModelViewMatrix)
{
  if(op)
  {
    m_fbo->setActive(true);
    m_shader->setActive(true);
    glUniformMatrix4fv(m_lightModeleViewMatrixLoc, 1, GL_FALSE, lightModelViewMatrix);
    activateTextures();
  }else 
  {
    m_shader->setActive(false);
    m_fbo->setActive(false);
  }
}


