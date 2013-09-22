/**
 *	Eduardo Ceretta Dalla Favera with modifications of Cesar Tadeu Pozzer
 *  eduardo.ceretta@gmail.com, pozzer3@gmail.com
 *  Mar 2010  
 *
 *  OpenGL Shader utility class.
 */
#include <iostream>
#include <sstream>
#include <cassert>
#include "defines.h"
#include "GLShader.h"
#include "GLError.h"

const unsigned int GLShader::MaxUniformMemorySize(65536u);
const char GLShader::WildCardStr[] = "$REPLACE$";

GLShader::GLShader()
:m_uniformMemorySize(0)
,m_vertFileName(NULL)
,m_fragFileName(NULL)
{
}

GLShader::GLShader(char* vertexShaderFile, char* fragShaderFile)
:m_uniformMemorySize(0)
,m_vertFileName(NULL)
,m_fragFileName(NULL)
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

  if(m_vertFileName)
    free(m_vertFileName);
  if(m_fragFileName)
    free(m_fragFileName);
}

void GLShader::setShaderFiles(char* vert, char* frag){
  m_vertFileName = strdup(vert);
  m_fragFileName = strdup(frag);
  loadFiles(m_vertFileName, m_fragFileName);
}

void GLShader::setActive(bool active){
  MyAssert("Out of UniformMemory", m_uniformMemorySize <= MaxUniformMemorySize);
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
  map<string, pair<GLfloat*, int> > ::iterator itFloatArray = m_uniformFloatArrayVec.begin();
  map<string, pair<GLfloat*, int> > ::iterator itFloat2Array = m_uniformFloat2ArrayVec.begin();

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
  
  for (; itFloatArray != m_uniformFloatArrayVec.end(); ++itFloatArray)
    setUniformFloatArray((char*)itFloatArray->first.c_str(), (itFloatArray->second).first, (itFloatArray->second).second);
  
  for (; itFloat2Array != m_uniformFloat2ArrayVec.end(); ++itFloat2Array)
    setUniformFloatArray((char*)itFloat2Array->first.c_str(), (itFloat2Array->second).first, (itFloat2Array->second).second);

  setActive(false);
}


void GLShader::setUniformTexture(char* name, GLuint value )
{
  GLint loc = glGetUniformLocation(m_shaderProg, name);
  if(m_successfulLoad && loc == -1)
    printf("Variable %s do not exist or is not used!\n", name);
  else
  {
    string name_str = string(name);
    if(m_uniformIntVec.find(name_str) == m_uniformIntVec.end())
      m_uniformMemorySize += sizeof(GLuint);  
    m_uniformIntVec[name_str] = value;
    glUniform1i(loc, value);
  }
}

void GLShader::setUniformInt(char* name, GLint value )
{
  GLint loc = glGetUniformLocation(m_shaderProg, name);
  if(m_successfulLoad && loc == -1)
    printf("Variable %s do not exist or is not used!\n", name);
  else
  {
    string name_str = string(name);
    if(m_uniformIntVec.find(name_str) == m_uniformIntVec.end())
      m_uniformMemorySize += sizeof(GLuint);
    m_uniformIntVec[name_str] = value;
    glUniform1i(loc, value);
  }
}

void GLShader::setUniformFloat(char* name, GLfloat value )
{
  GLint loc = glGetUniformLocation(m_shaderProg, name);
  if(m_successfulLoad && loc == -1)
    printf("Variable %s do not exist or is not used!\n", name);
  else
  {
    string name_str = string(name);
    if(m_uniformFloatVec.find(name_str) == m_uniformFloatVec.end())
      m_uniformMemorySize += sizeof(GLfloat);
    m_uniformFloatVec[name_str] = value;
    glUniform1f(loc, value);
  }
}

void GLShader::setUniformFloatArray(char* name, GLfloat *value, int n)
{
  GLint loc = glGetUniformLocation(m_shaderProg, name);
  if(m_successfulLoad && loc == -1)
    printf("Variable %s do not exist or is not used!\n", name);
  else
  {
    string name_str = string(name);
     if(m_uniformFloatArrayVec.find(name_str) == m_uniformFloatArrayVec.end())
      m_uniformMemorySize += n*sizeof(GLfloat);
    else if(m_uniformFloatArrayVec[name_str].second != n)
    {
      m_uniformMemorySize -= m_uniformFloatArrayVec[name_str].second*sizeof(GLfloat);
      m_uniformMemorySize += n*sizeof(GLfloat);
    }
    m_uniformFloatArrayVec[name_str] = pair<GLfloat*, int> (value, n);
    glUniform1fvARB(loc, n, value);
  }
}

void GLShader::setUniformFloat2Array(char* name, GLfloat *value, int n)
{
  GLint loc = glGetUniformLocation(m_shaderProg, name);
  if(m_successfulLoad && loc == -1)
    printf("Variable %s do not exist or is not used!\n", name);
  else
  {
    string name_str = string(name);
     if(m_uniformFloat2ArrayVec.find(name_str) == m_uniformFloat2ArrayVec.end())
      m_uniformMemorySize += 2*n*sizeof(GLfloat);
    else if(m_uniformFloat2ArrayVec[name_str].second != n)
    {
      m_uniformMemorySize -= 2*m_uniformFloat2ArrayVec[name_str].second*sizeof(GLfloat);
      m_uniformMemorySize += 2*n*sizeof(GLfloat);
    }
    m_uniformFloat2ArrayVec[name_str] = pair<GLfloat*, int> (value, n);
    glUniform2fvARB(loc, n, value);
  }
}


void GLShader::setUniformVec3(char* name, Vector3 value )
{
  GLint loc = glGetUniformLocation(m_shaderProg, name);
  if(m_successfulLoad && loc == -1)
    printf("Variable %s do not exist or is not used!\n", name);
  else
  {
    string name_str = string(name);
    if(m_uniformVector3Vec.find(name_str) == m_uniformVector3Vec.end())
      m_uniformMemorySize += 3*sizeof(GLfloat);
    m_uniformVector3Vec[name_str] = value;
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
    string name_str = string(name);
    if(m_uniformVector3ArrayVec.find(name_str) == m_uniformVector3ArrayVec.end())
      m_uniformMemorySize += 3*n*sizeof(GLfloat);
    else if(m_uniformVector3ArrayVec[name_str].second != n)
    {
      m_uniformMemorySize -= 3*m_uniformVector3ArrayVec[name_str].second*sizeof(GLfloat);
      m_uniformMemorySize += 3*n*sizeof(GLfloat);
    }
    m_uniformVector3ArrayVec[name_str] = pair<GLfloat*, int> (value, n);
    glUniform3fvARB(loc, n, value);
  }
}

void GLShader::setUniformMatrix4(char* name, const GLfloat* value )
{
  GLint loc = glGetUniformLocation(m_shaderProg, name);
  if(m_successfulLoad && loc == -1)
    printf("Variable %s do not exist or is not used!\n", name);
  else
  {
    string name_str = string(name);
    if(m_uniformMatrixVec.find(name_str) == m_uniformMatrixVec.end())
      m_uniformMemorySize += 16*sizeof(GLfloat);
    m_uniformMatrixVec[name_str] = value;
    glUniformMatrix4fv(loc, 1, GL_FALSE, value);
  }
}


void GLShader::addReplaceDefine (string defineName, int defineValue)
{
  ostringstream ss;
  ss << defineValue;
  pair<string, string> define(defineName, ss.str());
  
  m_defineReplacementList.push_back(define);
}

void GLShader::addReplaceDefine (string defineName, bool defineValue)
{
  ostringstream ss;
  ss << (defineValue == true? 1 : 0);
  pair<string, string> define(defineName, ss.str());

  m_defineReplacementList.push_back(define);
}

void GLShader::addReplaceDefine (string defineName, float defineValue)
{
  ostringstream ss;
  ss << defineValue;
  pair<string, string> define(defineName, ss.str());

  m_defineReplacementList.push_back(define);
}

void GLShader::addReplaceDefine (string defineName, string defineValue)
{
  pair<string, string> define(defineName, defineValue);

  m_defineReplacementList.push_back(define);
}


void GLShader::loadFiles(char* vertexShaderFile, char* fragShaderFile){

  char *vs = NULL,*fs = NULL;
  glewInit();
  m_shaderVert = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
  m_shaderFrag = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);	

  vs = textFileRead(vertexShaderFile);
  fs = textFileRead(fragShaderFile);

  replaceDefines(vs);
  replaceDefines(fs);

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

  replaceDefines(vs);
  replaceDefines(fs);

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

void GLShader::replaceDefines (char* shaderText)
{
  char* ptr(shaderText);
  char name[300];
  char val[300];

  while(ptr && *ptr != '\0')
  {
    ptr = strstr(ptr, "#define");
    if(ptr != NULL)
    {
      int i = sscanf(ptr, "#define %s %30s", name, val);

      if(!strcmp(val, WildCardStr))
      {
        vector<pair<string, string> > :: iterator defIt(m_defineReplacementList.begin());
        for(; defIt != m_defineReplacementList.end(); ++defIt)
        {
          if(!strcmp(name, defIt->first.c_str()))
          {
            char* replace = strstr(ptr, WildCardStr);
            if(replace != NULL)
            {
              printf("Define Replace: %s = %s\n", name, defIt->second.c_str());

              memset(replace, ' ', sizeof(WildCardStr)-1);
              memcpy(replace, defIt->second.c_str(), std::min(sizeof(WildCardStr)-1, defIt->second.length()));
              break;
            }
          }
        }
      }
      ptr++;
    }
  }
  printf("\n");
}
