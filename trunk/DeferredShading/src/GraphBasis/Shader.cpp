#include "Shader.h"


Shader::Shader(char* vertexShaderFile, char* fragShaderFile){
	LoadFiles(vertexShaderFile, fragShaderFile);
}

Shader::~Shader(){
	
	glDetachObjectARB(m_shaderProg, m_shaderVert);
	glDeleteObjectARB(m_shaderVert); 

	glDetachObjectARB(m_shaderProg, m_shaderFrag);
	glDeleteObjectARB(m_shaderFrag); 

	glDeleteObjectARB(m_shaderProg); 
	
}

void Shader::setActive(bool active){
	if(active)
		glUseProgramObjectARB(m_shaderProg);
	else
		glUseProgramObjectARB( 0 );
 
}

GLint Shader::getUniformLocation(char* name){

	return glGetUniformLocation(m_shaderProg, name);

}

void Shader::LoadFiles(char* vertexShaderFile, char* fragShaderFile){
	
	char *vs = NULL,*fs = NULL;
	
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

		std::cout << "==================\n";
		std::cout << "Log for shader files: " << vertexShaderFile << ", " << fragShaderFile << "\n";

		printInfoLog(m_shaderVert);
		printInfoLog(m_shaderFrag);
		
		
		m_shaderProg = glCreateProgramObjectARB();
		
		glAttachObjectARB(m_shaderProg,m_shaderVert);
		glAttachObjectARB(m_shaderProg,m_shaderFrag);

		glLinkProgramARB(m_shaderProg);
		printInfoLog(m_shaderProg);
		std::cout << "\n==================\n";
	
	}
	else if(fs == NULL){
		const char * vv = vs;
		const char * ff = fs;

		glShaderSourceARB(m_shaderVert, 1, &vv,NULL);

		free(vs);

		glCompileShaderARB(m_shaderVert);

		//printInfoLog(m_shaderVert);

		m_shaderProg = glCreateProgramObjectARB();
		
		glAttachObjectARB(m_shaderProg,m_shaderVert);

		glLinkProgramARB(m_shaderProg);
		printInfoLog(m_shaderProg);
		
	}

}