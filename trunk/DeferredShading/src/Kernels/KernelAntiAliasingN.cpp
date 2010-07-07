#include "KernelAntiAliasingN.h"

KernelAntiAliasingN::KernelAntiAliasingN(int width, int height, GLuint colorTexId, GLuint factorTexId)
: KernelBase("./resources/Shaders/antialiasingN.vert", "./resources/Shaders/antialiasingN.frag", width, height){
  //Output
  //m_colorTexId = addOutput(0);
  addOutput(0, colorTexId);


  //m_original_colorTexId = colorTexId;
  //Input
  m_shader->setActive(true);
  addInputTexture(GL_TEXTURE_2D, "colorTex", colorTexId);
  addInputTexture(GL_TEXTURE_2D, "factorTex", factorTexId);

  m_shader->setActive(false);
}

KernelAntiAliasingN::~KernelAntiAliasingN(){

}


void KernelAntiAliasingN::renderShader()
{
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0, 1, 0, 1);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  
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

void KernelAntiAliasingN::step(int i){
  //if(!i)
  //  m_inputTextures[0].second = m_original_colorTexId;
  //else m_inputTextures[0].second = m_colorTexId;

  m_fbo->setActive(true);
  m_shader->setActive(true);
    
  activateTextures();
  renderQuad();

  m_shader->setActive(false);
  m_fbo->setActive(false);
}

void KernelAntiAliasingN::setActive(bool op)
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


