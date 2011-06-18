#ifndef Shader_H
#define Shader_H

#include <iostream>
#include <map>
#include <string>
#include "GraphBasis/Vector3.h"
#include "GL/glew.h"

#include "textfile.h"

using namespace std;

class Shader{
	char* m_fragFile;
  char* m_vertFile;

  map<string, GLint>    m_uniformIntVec;
  map<string, GLfloat>  m_uniformFloatVec;
  map<string, Vector3>  m_uniformVector3Vec;
  map<string, const GLfloat*> m_uniformMatrixVec;
   
public:
	Shader(){};
	Shader(char*, char*);
	~Shader();


	void setActive(bool active);
	//GLint getUniformLocation(char* name);

  void setUniformTexture(char* name, GLuint id);
  void setUniformVec3(char* name, Vector3 value);
  void setUniformInt(char* name, GLint value);
  void setUniformFloat(char* name, GLfloat value);
  void setUniformMatrix4(char* name, const GLfloat* value);

  void reload();
	


protected:
	void LoadFiles(char*, char*);
  void ReloadLoadFiles(char* vertexShaderFile, char* fragShaderFile);
 
	GLhandleARB m_shaderProg;
	GLhandleARB m_shaderVert;
	GLhandleARB m_shaderFrag;
  GLint m_successfulLoad;

	

};


#endif