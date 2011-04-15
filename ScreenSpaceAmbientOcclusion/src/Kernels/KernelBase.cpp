#include "Kernels/KernelBase.h"

KernelBase::KernelBase(){

}

KernelBase::KernelBase(char* vert, char* frag, int width, int height){
	m_shader = new Shader(vert, frag);
	m_fbo = new FrameBufferObject(width, height);
  //m_fbo->attachToDepthBuffer(BufferType::RenderBufferObject);
}

KernelBase::~KernelBase(){

	delete m_shader;
	delete m_fbo;

}

GLuint KernelBase::addInputTexture(GLenum textureDimension, char* name, GLuint id){

  GLuint loc = m_shader->getUniformLocation(name);
  glUniform1iARB(loc, m_inputTextures.size());
  m_inputTextures.push_back(std::pair<GLenum, GLuint>(textureDimension, id));

  return loc;
}

GLuint KernelBase::setInputTexture(GLenum textureDimension, char* name, GLuint id, int index){

  GLuint loc = m_shader->getUniformLocation(name);
  glUniform1iARB(loc, index);
  m_inputTextures[index].first = textureDimension;
  m_inputTextures[index].second = id;

  return loc;
}

GLuint KernelBase::addInputVec3(char* name, Vector3 value){
	
	GLuint loc = m_shader->getUniformLocation(name);
	glUniform3fARB(loc, value.x, value.y, value.z);

	return loc;
}

GLuint KernelBase::addInputFloat( char* name, GLfloat value )
{
  GLuint loc = m_shader->getUniformLocation(name);
  glUniform1f(loc, value);

  return loc;
}


GLuint KernelBase::addInputMatrix4( char* name, const GLfloat* value )
{
  GLuint loc = m_shader->getUniformLocation(name);
  glUniformMatrix4fv(loc, 1,GL_FALSE, value);

  return loc;
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



GLuint KernelBase::getOutputTexture(int index){
	return m_outputTextures.at(index);
}

void KernelBase::activateTextures(){

  for(int i=0; i<m_inputTextures.size(); i++){
		glActiveTextureARB(GL_TEXTURE0 + i);
		glBindTexture(m_inputTextures.at(i).first, m_inputTextures.at(i).second);
	}
}


void KernelBase::renderQuad(){
  glBegin(GL_QUADS);
    glTexCoord2f(0,0); glVertex3f(0,0,0); 
    glTexCoord2f(1,0); glVertex3f(1,0,0); 
    glTexCoord2f(1,1); glVertex3f(1,1,0); 
    glTexCoord2f(0,1); glVertex3f(0,1,0); 
  glEnd();
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
	m_shader->setActive(true);
	//activateTextures();
	renderScreenQuad();
	m_shader->setActive(false);
	m_fbo->setActive(false);
}

void KernelBase::setActive( bool op )
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

void KernelBase::setActiveShaderOnly( bool op )
{
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
