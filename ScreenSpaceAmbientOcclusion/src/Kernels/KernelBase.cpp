/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Oct 2010  
 *
 *  Base Class for rendering kernels definition.
 *  A kernel is composed by a FBO, a shader and input and output textures.
 */

#include "Kernels/KernelBase.h"
#include "GLUtils/GLFrameBufferObject.h"
#include "GLUtils/GLShader.h"

KernelBase::KernelBase(){

}

KernelBase::KernelBase(char* path, char* vert, char* frag, int width, int height){
  if(!vert || !frag)
    m_shader = NULL;
  else
  {
    string v = string(path) + vert;
    string f = string(path) + frag;
    m_shader = new GLShader((char*)v.c_str(), (char*)f.c_str());
  }
  
  m_fbo = new GLFrameBufferObject(width, height);
}

KernelBase::KernelBase(char* vert, char* frag, int width, int height){
  if(!vert || !frag)
    m_shader = NULL;
  else
	  m_shader = new GLShader(vert, frag);
	m_fbo = new GLFrameBufferObject(width, height);
}

KernelBase::~KernelBase(){
  if(m_shader)
	  delete m_shader;
	delete m_fbo;
}

void KernelBase::reloadShader()
{
  if(m_shader)
    m_shader->reload();
}
void KernelBase::setActive( bool op )
{
  if(op)
  {
    m_fbo->setActive(true);
    if(m_shader)
      m_shader->setActive(true);
    activateTextures();
  }else 
  {
    if(m_shader)
      m_shader->setActive(false);
    m_fbo->setActive(false);
  }
}

void KernelBase::setActiveShaderOnly( bool op )
{
  if(!m_shader) return;
  if(op)
  {
    m_shader->setActive(true);
    activateTextures();
  }else 
  {
    m_shader->setActive(false);
  }
}

void KernelBase::renderOutput( int texIndex /*= 0*/ )
{
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0, 1, 0, 1);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glEnable(GL_TEXTURE_2D);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, getOutputTexture(texIndex));
  glColor3f(1,1,1);
  glBegin(GL_QUADS);
  glTexCoord2f(0,0); glVertex3f(0,0,0);
  glTexCoord2f(1,0); glVertex3f(1,0,0);
  glTexCoord2f(1,1); glVertex3f(1,1,0);
  glTexCoord2f(0,1); glVertex3f(0,1,0);
  glEnd();
  glBindTexture(GL_TEXTURE_2D, 0);
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}

void KernelBase::renderShader()
{
  glPushAttrib(GL_ENABLE_BIT);
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
  glPopAttrib();
}

void KernelBase::step(){
  m_fbo->setActive(true);
  if(m_shader)
    m_shader->setActive(true);
  renderScreenQuad();
  if(m_shader)
    m_shader->setActive(false);
  m_fbo->setActive(false);
}


GLuint KernelBase::getOutputTexture(int index){
  return m_outputTextures.at(index);
}

void KernelBase::activateTextures(){

  for(int i=0; i<m_inputTextures.size(); i++){
    glActiveTextureARB(GL_TEXTURE0 + i);
    glBindTexture(m_inputTextures.at(i).first, m_inputTextures.at(i).second);
  }
}

void KernelBase::renderScreenQuad(){
  glPushAttrib(GL_ENABLE_BIT);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0, 1, 0, 1);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();


  glEnable(GL_TEXTURE_1D);
  glEnable(GL_TEXTURE_2D);

  activateTextures();
  glColor3f(1,1,1);
  glBegin(GL_QUADS);
  glTexCoord2f(0,0); glVertex3f(0,0,0);
  glTexCoord2f(1,0); glVertex3f(1,0,0);
  glTexCoord2f(1,1); glVertex3f(1,1,0);
  glTexCoord2f(0,1); glVertex3f(0,1,0);
  glEnd();


  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glPopAttrib();
}

void KernelBase::addInputTexture(GLenum textureDimension, char* name, GLuint id){
  m_shader->setUniformTexture(name, m_inputTextures.size());
  m_inputTextures.push_back(std::pair<GLenum, GLuint>(textureDimension, id));
}

void KernelBase::setInputTexture(GLenum textureDimension, char* name, GLuint id, int index){
  m_shader->setUniformTexture(name, index);
  m_inputTextures[index].first = textureDimension;
  m_inputTextures[index].second = id;
}

void KernelBase::addInputVec3(char* name, Vector3 value){
	m_shader->setUniformVec3(name, value);
}

void KernelBase::addInputFloat( char* name, GLfloat value )
{
  m_shader->setUniformFloat(name, value);
}

void KernelBase::addInputInt( char* name, GLint value )
{
  m_shader->setUniformInt(name, value);
}


void KernelBase::addInputMatrix4( char* name, const GLfloat* value )
{
  m_shader->setUniformMatrix4(name, value);
}


GLuint KernelBase::addOutput(int index, GLuint textureId){
  GLuint id = m_fbo->attachToColorBuffer(bufferType::Texture, index, textureId);
  if(index < m_outputTextures.size())
    m_outputTextures[index] = id;
  else 
    m_outputTextures.push_back(id);
  return id;
}

GLuint KernelBase::setOutput(int index, GLuint textureId){
  GLuint id = m_fbo->attachToColorBuffer(bufferType::Texture, index, textureId);
  m_outputTextures[index] = id;
  return id;
}

