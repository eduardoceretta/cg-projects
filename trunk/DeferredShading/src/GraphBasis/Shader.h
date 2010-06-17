#ifndef Shader_H
#define Shader_H

#include <iostream>
#include "GL/glew.h"

#include "textfile.h"


class Shader{
	
	
public:
	Shader(){};
	Shader(char*, char*);
	~Shader();
	void setActive(bool active);
	GLint getUniformLocation(char* name);
	

	


protected:
	void LoadFiles(char*, char*);

	GLhandleARB m_shaderProg;
	GLhandleARB m_shaderVert;
	GLhandleARB m_shaderFrag;
	
	

};


#endif