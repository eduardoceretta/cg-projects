/**
 *	Eduardo Ceretta Dalla Favera with modifications of Cesar Tadeu Pozzer
 *  eduardo.ceretta@gmail.com, pozzer3@gmail.com
 *  Mar 2010  
 *
 *  OpenGL Shader utility class.
 */
#ifndef _GL_SHADER_H_
#define _GL_SHADER_H_

#include <map>
#include <utility>
#include <string>

#include "MathUtils/Vector3.h"
#include "GL/glew.h"


using namespace std;

class GLShader{
  /**
   * Fragment shader file name
   */
	char* m_fragFileName;
  
  /**
   * Vertex shader file name
   */
  char* m_vertFileName;

  /**
   * Shader uniform variables.
   *  Maps the variable name with the value.
   *  Necessary for the correct reload of the shader.
   */
  map<string, GLint>    m_uniformIntVec;
  map<string, GLfloat>  m_uniformFloatVec;
  map<string, Vector3>  m_uniformVector3Vec;
  map<string, const GLfloat*> m_uniformMatrixVec;

  map<string, pair<GLfloat*, int> > m_uniformVector3ArrayVec;

  /**
   * OpenGL Shader Program.
   */
  GLhandleARB m_shaderProg;

  /**
   * OpenGL VertexShader.
   */
  GLhandleARB m_shaderVert;
  /**
   * OpenGL FragmentShader.
   */
  GLhandleARB m_shaderFrag;

  /**
   * Indicates if the shader was successfully loaded.
   */
  GLint m_successfulLoad;
   
public:
  /**
   * Simple Constructor
   */
	GLShader(){};
  
  /**
   * Receives the filenames of the shader files and create the OpenGL Shader object
   *  Read the files, creates the shader and attach the shader sources
   */
	GLShader(char* vert, char* frag);
  
  /**
   * Deletes allocated memory of the main memory and GPU's memory
   */
	~GLShader();

  /**
   * Activates and desactivates the shader.
   */
	void setActive(bool active);

  /**
   * Set a uniform variable to the shader.
   */
  void setUniformTexture(char* name, GLuint id);
  void setUniformVec3(char* name, Vector3 value);
  void setUniformInt(char* name, GLint value);
  void setUniformFloat(char* name, GLfloat value);
  void setUniformMatrix4(char* name, const GLfloat* value);

  void setUniformVec3Array(char* name, GLfloat *value, int n);

  /**
   * Reloads the shader files.
   *  Clear the current shader, read the files, 
   *  creates the shader, attach the shader sources and
   *  sets the uniform variables to the shader.
   */
  void reload();

protected:
  /**
   * Read the files, creates the shader and attach the shader sources
   */
	void loadFiles(char* vertexShaderFileName, char* fragShaderFileName);

  /**
   *  Clear the current shader, read the files, 
   *  creates the shader, attach the shader sources and
   *  sets the uniform variables to the shader.
   */
  void reloadLoadFiles(char* vertexShaderFileName, char* fragShaderFileName);

  /**
   * Prints shader info.
   */
  bool printInfoLog(GLhandleARB obj);

  /**
   * Read text file.
   */
  char* textFileRead(char *fn);

  /**
   * Write text file.
   */
  int textFileWrite(char *fname, char *s);
 
};


#endif