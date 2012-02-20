/**
 *	Eduardo Ceretta Dalla Favera with modifications of Cesar Tadeu Pozzer
 *  eduardo.ceretta@gmail.com, pozzer3@gmail.com
 *  Mar 2010  
 *
 *  OpenGL Shader utility class.
 */
#include <iostream>
#include <assert.h>
#include "GLShader.h"

GLShader::GLShader(char* vertexShaderFile, char* fragShaderFile)
{
  m_vertFileName = strdup(vertexShaderFile);
  m_fragFileName = strdup(fragShaderFile);
	loadFiles(m_vertFileName, m_fragFileName);
}

GLShader::~GLShader(){
	
	glDetachObjectARB(m_shaderProg, m_shaderVert);
	glDeleteObjectARB(m_shaderVert); 

	glDetachObjectARB(m_shaderProg, m_shaderFrag);
	glDeleteObjectARB(m_shaderFrag); 

	glDeleteObjectARB(m_shaderProg); 

  free(m_vertFileName);
  free(m_fragFileName);
}

void GLShader::setActive(bool active){
	if(active)
		glUseProgramObjectARB(m_shaderProg);
	else
		glUseProgramObjectARB( 0 );
}



void GLShader::reload()
{
	reloadLoadFiles(m_vertFileName, m_fragFileName);
  setActive(true);

  map<string, GLint> ::iterator itInt = m_uniformIntVec.begin();
  map<string, GLfloat> ::iterator itFloat = m_uniformFloatVec.begin();
  map<string, Vector3> ::iterator itVector3 = m_uniformVector3Vec.begin();
  map<string, const GLfloat*> ::iterator itMatrix = m_uniformMatrixVec.begin();
  map<string, pair<GLfloat*, int> > ::iterator itVector3Array = m_uniformVector3ArrayVec.begin();

  for (; itInt != m_uniformIntVec.end(); ++itInt)
   setUniformInt((char*)itInt->first.c_str(), itInt->second);

  for (; itFloat != m_uniformFloatVec.end(); ++itFloat)
    setUniformFloat((char*)itFloat->first.c_str(), itFloat->second);

  for (; itVector3 != m_uniformVector3Vec.end(); ++itVector3)
    setUniformVec3((char*)itVector3->first.c_str(), itVector3->second);

  for (; itMatrix != m_uniformMatrixVec.end(); ++itMatrix)
    setUniformMatrix4((char*)itMatrix->first.c_str(), itMatrix->second);
  
  for (; itVector3Array != m_uniformVector3ArrayVec.end(); ++itVector3Array)
    setUniformVec3Array((char*)itVector3Array->first.c_str(), (itVector3Array->second).first, (itVector3Array->second).second);

  setActive(false);
}


void GLShader::setUniformTexture(char* name, GLuint value )
{
  GLint loc = glGetUniformLocation(m_shaderProg, name);
  if(m_successfulLoad && loc == -1)
    printf("Variable %s do not exist or is not used!\n", name);
  else
  {
    m_uniformIntVec[string(name)] = value;
    glUniform1i(loc, value);
  }
}

void GLShader::setUniformVec3( char* name, Vector3 value )
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

void GLShader::setUniformVec3Array(char* name, GLfloat *value, int n)
{
  GLint loc = glGetUniformLocation(m_shaderProg, name);
  if(m_successfulLoad && loc == -1)
    printf("Variable %s do not exist or is not used!\n", name);
  else
  {
    m_uniformVector3ArrayVec[string(name)] = pair<GLfloat*, int> (value, n);
    glUniform3fvARB(loc, n, value);
  }
}

void GLShader::setUniformInt( char* name, GLint value )
{
  GLint loc = glGetUniformLocation(m_shaderProg, name);
  if(m_successfulLoad && loc == -1)
    printf("Variable %s do not exist or is not used!\n", name);
  else
  {
    m_uniformIntVec[string(name)] = value;
    glUniform1i(loc, value);
  }
}

void GLShader::setUniformFloat( char* name, GLfloat value )
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

void GLShader::setUniformMatrix4( char* name, const GLfloat* value )
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


void GLShader::loadFiles(char* vertexShaderFile, char* fragShaderFile){

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

    std::cout << "Reading Shaders: " << vertexShaderFile << ", " << fragShaderFile << " ..." <<std::endl;

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

    m_shaderProg = glCreateProgramObjectARB();

    glAttachObjectARB(m_shaderProg,m_shaderVert);

    glLinkProgramARB(m_shaderProg);

    m_successfulLoad = GL_FALSE;
    glGetObjectParameterivARB(m_shaderProg, GL_OBJECT_LINK_STATUS_ARB, &m_successfulLoad);

    printInfoLog(m_shaderProg);
    std::cout << std::endl;
  }
}


void GLShader::reloadLoadFiles(char* vertexShaderFile, char* fragShaderFile){

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

    glAttachObjectARB(m_shaderProg,m_shaderVert);

    glLinkProgramARB(m_shaderProg);

    m_successfulLoad = GL_FALSE;
    glGetObjectParameterivARB(m_shaderProg, GL_OBJECT_LINK_STATUS_ARB, &m_successfulLoad);

    printInfoLog(m_shaderProg);
    std::cout << std::endl;
  }

}

bool GLShader::printInfoLog(GLhandleARB obj)
{
  int infologLength = 0;
  int charsWritten  = 0;
  char *infoLog;

  glGetObjectParameterivARB(obj, GL_OBJECT_INFO_LOG_LENGTH_ARB,
    &infologLength);

  if (infologLength > 1)
  {
    infoLog = (char *)malloc(infologLength);
    glGetInfoLogARB(obj, infologLength, &charsWritten, infoLog);
    printf("%s\n",infoLog);
    printf("==================\n");
    free(infoLog);
    return true;
  }
  return false;
}

char* GLShader::textFileRead(char *fn)
{
  FILE *fp;
  char *content = NULL;

  int count=0;

  if (fn != NULL) {
    fp = fopen(fn,"rt");

    if (fp != NULL) {

      fseek(fp, 0, SEEK_END);
      count = ftell(fp);
      rewind(fp);

      if (count > 0) {
        content = (char *)malloc(sizeof(char) * (count+1));
        count = fread(content,sizeof(char),count,fp);
        content[count] = '\0';
      }
      fclose(fp);
    }
    else{
      printf("Could not open file: %s\n",fn);
      assert(false);
    }
  }
  return content;
}

int GLShader::textFileWrite(char *fn, char *s)
{

  FILE *fp;
  int status = 0;

  if (fn != NULL) {
    fp = fopen(fn,"w");

    if (fp != NULL) {

      if (fwrite(s,sizeof(char),strlen(s),fp) == strlen(s))
        status = 1;
      fclose(fp);
    }
  }
  return(status);
}
