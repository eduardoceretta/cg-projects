#include "KernelAntiAliasing.h"

KernelAntiAliasing::KernelAntiAliasing(int width, int height, GLuint normalTexId)
: KernelBase("./resources/Shaders/antialiasing.vert", "./resources/Shaders/antialiasing.frag", width, height){
  //Output
  m_factorTexId = addOutput(0);
  //Input
  m_shader->setActive(true);
  addInputTexture(GL_TEXTURE_2D, "normalTex", normalTexId);
  //addInputTexture(GL_TEXTURE_2D, "colorTex", colorTexId);

  //addInputFloat("lightTexSize", lightTexSize);
  m_shader->setActive(false);
}

KernelAntiAliasing::~KernelAntiAliasing(){

}


void KernelAntiAliasing::renderShader()
{
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0, 1, 0, 1);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glEnable(GL_TEXTURE_1D);
  glEnable(GL_TEXTURE_2D);

  m_shader->setActive(true);

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
}

void KernelAntiAliasing::step(){
  m_fbo->setActive(true);
  m_shader->setActive(true);
  activateTextures();
  renderQuad();
  m_shader->setActive(false);
  m_fbo->setActive(false);
}

void KernelAntiAliasing::setActive(bool op)
{
  if(op)
  {
    m_fbo->setActive(true);
    m_shader->setActive(true);
    activateTextures();
  }else 
  {
    m_shader->setActive(false);
    m_fbo->setActive(false);
  }
}


GLuint KernelAntiAliasing::getTexIdFactor() const 
{
  return m_factorTexId; 
}