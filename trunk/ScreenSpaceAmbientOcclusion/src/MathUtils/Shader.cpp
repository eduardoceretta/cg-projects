#include "Shader.h"
#include <string.h>


Shader::Shader(char* vertexShaderFile, char* fragShaderFile)
{
  m_vertFile = strdup(vertexShaderFile);
  m_fragFile = strdup(fragShaderFile);
	LoadFiles(m_vertFile, m_fragFile);
}

Shader::~Shader(){
	
	glDetachObjectARB(m_shaderProg, m_shaderVert);
	glDeleteObjectARB(m_shaderVert); 

	glDetachObjectARB(m_shaderProg, m_shaderFrag);
	glDeleteObjectARB(m_shaderFrag); 

	glDeleteObjectARB(m_shaderProg); 

  free(m_vertFile);
  free(m_fragFile);
}

void Shader::setActive(bool active){
	if(active)
		glUseProgramObjectARB(m_shaderProg);
	else
		glUseProgramObjectARB( 0 );
 
}



void Shader::LoadFiles(char* vertexShaderFile, char* fragShaderFile){
	
	char *vs = NULL,*fs = NULL;
	glewInit();
	m_shaderVert = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	m_shaderFrag = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);	

	vs = textFileRead(vertexShaderFile);
	fs = textFileRead(fragShaderFile);

	if(vs != NULL && fs != NULL){
		const char * vv = vs;
		const char * ff = fs;

		glShaderSourceARB(m_shaderVert, 1, &vv,NULL);
		glShaderSourceARB(m_shaderFrag, 1, &ff,NULL);

		free(vs);free(fs);

		glCompileShaderARB(m_shaderVert);
		glCompileShaderARB(m_shaderFrag);

    std::cout << "Loading shader files: " << vertexShaderFile << ", " << fragShaderFile <<std::endl;

		//printInfoLog(m_shaderVert);
		//printInfoLog(m_shaderFrag);
  //  std::cout << "-----------------\n";

		m_shaderProg = glCreateProgramObjectARB();
		
		glAttachObjectARB(m_shaderProg,m_shaderVert);
		glAttachObjectARB(m_shaderProg,m_shaderFrag);

		glLinkProgramARB(m_shaderProg);

    m_successfulLoad = GL_FALSE;
    glGetObjectParameterivARB(m_shaderProg, GL_OBJECT_LINK_STATUS_ARB, &m_successfulLoad);

		printInfoLog(m_shaderProg);
    std::cout << std::endl;
	}
	else if(fs == NULL){
		const char * vv = vs;
		const char * ff = fs;

		glShaderSourceARB(m_shaderVert, 1, &vv,NULL);

		free(vs);

		glCompileShaderARB(m_shaderVert);
    std::cout << "Loading shader files: " << vertexShaderFile <<std::endl;

		//printInfoLog(m_shaderVert);

		m_shaderProg = glCreateProgramObjectARB();
		
		glAttachObjectARB(m_shaderProg,m_shaderVert);

		glLinkProgramARB(m_shaderProg);

    m_successfulLoad = GL_FALSE;
    glGetObjectParameterivARB(m_shaderProg, GL_OBJECT_LINK_STATUS_ARB, &m_successfulLoad);

		printInfoLog(m_shaderProg);
		std::cout << std::endl;
	}

}


void Shader::ReloadLoadFiles(char* vertexShaderFile, char* fragShaderFile){

  char *vs = NULL,*fs = NULL;

  vs = textFileRead(vertexShaderFile);
  fs = textFileRead(fragShaderFile);

  if(vs != NULL && fs != NULL){
    const char * vv = vs;
    const char * ff = fs;

    glDetachObjectARB(m_shaderProg,m_shaderVert);
    glDetachObjectARB(m_shaderProg,m_shaderFrag);
    glDeleteObjectARB(m_shaderVert);
    glDeleteObjectARB(m_shaderFrag);

    m_shaderVert = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
    m_shaderFrag = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);	

    glShaderSourceARB(m_shaderVert, 1, &vv, NULL);
    glShaderSourceARB(m_shaderFrag, 1, &ff, NULL);

    free(vs);
    free(fs);

    glCompileShaderARB(m_shaderVert);
    glCompileShaderARB(m_shaderFrag);
  


    std::cout << "Reloading shader files" << vertexShaderFile << ", " << fragShaderFile <<std::endl;

    //printInfoLog(m_shaderVert);
    //printInfoLog(m_shaderFrag);
    //std::cout << "-----------------\n";

    glAttachObjectARB(m_shaderProg,m_shaderVert);
    glAttachObjectARB(m_shaderProg,m_shaderFrag);

    glLinkProgramARB(m_shaderProg);

    m_successfulLoad = GL_FALSE;
    glGetObjectParameterivARB(m_shaderProg, GL_OBJECT_LINK_STATUS_ARB, &m_successfulLoad);

    printInfoLog(m_shaderProg);
    std::cout << std::endl;
  }
  else if(fs == NULL){
    const char * vv = vs;
    const char * ff = fs;

    glDetachObjectARB(m_shaderProg,m_shaderVert);
    glDeleteObjectARB(m_shaderVert);

    m_shaderVert = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);

    glShaderSourceARB(m_shaderVert, 1, &vv,NULL);

    free(vs);

    glCompileShaderARB(m_shaderVert);

    std::cout << "Reloading shader files" << vertexShaderFile <<std::endl;

    //printInfoLog(m_shaderVert);

    glAttachObjectARB(m_shaderProg,m_shaderVert);

    glLinkProgramARB(m_shaderProg);

    m_successfulLoad = GL_FALSE;
    glGetObjectParameterivARB(m_shaderProg, GL_OBJECT_LINK_STATUS_ARB, &m_successfulLoad);

    printInfoLog(m_shaderProg);
    std::cout << std::endl;
  }

}

void Shader::reload()
{
	ReloadLoadFiles(m_vertFile, m_fragFile);
  setActive(true);

  map<string, GLint> ::iterator itInt = m_uniformIntVec.begin();
  map<string, GLfloat> ::iterator itFloat = m_uniformFloatVec.begin();
  map<string, Vector3> ::iterator itVector3 = m_uniformVector3Vec.begin();
  map<string, const GLfloat*> ::iterator itMatrix = m_uniformMatrixVec.begin();

  for (; itInt != m_uniformIntVec.end(); ++itInt)
   setUniformInt((char*)itInt->first.c_str(), itInt->second);

  for (; itFloat != m_uniformFloatVec.end(); ++itFloat)
    setUniformFloat((char*)itFloat->first.c_str(), itFloat->second);

  for (; itVector3 != m_uniformVector3Vec.end(); ++itVector3)
    setUniformVec3((char*)itVector3->first.c_str(), itVector3->second);

  for (; itMatrix != m_uniformMatrixVec.end(); ++itMatrix)
    setUniformMatrix4((char*)itMatrix->first.c_str(), itMatrix->second);

  setActive(false);
}


void Shader::setUniformTexture(char* name, GLuint value )
{
  GLint loc = glGetUniformLocation(m_shaderProg, name);
  if(m_successfulLoad && loc == -1)
    printf("Variable %s do not exist or is not used!\n", name);
  else
  {
    m_uniformIntVec[string(name)] = value;
    glUniform1iARB(loc, value);
  }
}

void Shader::setUniformVec3( char* name, Vector3 value )
{
  GLint loc = glGetUniformLocation(m_shaderProg, name);
  if(m_successfulLoad && loc == -1)
    printf("Variable %s do not exist or is not used!\n", name);
  else
  {
    m_uniformVector3Vec[string(name)] = value;
    glUniform3fARB(loc, value.x, value.y, value.z);
  }
}

void Shader::setUniformInt( char* name, GLint value )
{
  GLint loc = glGetUniformLocation(m_shaderProg, name);
  if(m_successfulLoad && loc == -1)
    printf("Variable %s do not exist or is not used!\n", name);
  else
  {
    m_uniformIntVec[string(name)] = value;
    glUniform1iARB(loc, value);
  }
}

void Shader::setUniformFloat( char* name, GLfloat value )
{
  GLint loc = glGetUniformLocation(m_shaderProg, name);
  if(m_successfulLoad && loc == -1)
    printf("Variable %s do not exist or is not used!\n", name);
  else
  {
    m_uniformFloatVec[string(name)] = value;
    glUniform1f(loc, value);
  }
}

void Shader::setUniformMatrix4( char* name, const GLfloat* value )
{
  GLint loc = glGetUniformLocation(m_shaderProg, name);
  if(m_successfulLoad && loc == -1)
    printf("Variable %s do not exist or is not used!\n", name);
  else
  {
    m_uniformMatrixVec[string(name)] = value;
    glUniformMatrix4fv(loc, 1, GL_FALSE, value);
  }
}