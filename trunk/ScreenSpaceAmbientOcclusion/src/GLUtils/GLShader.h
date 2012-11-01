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
#include <vector>

#include "MathUtils/Vector3.h"
#include "GL/glew.h"


using namespace std;

class GLShader{
  static const unsigned int MaxUniformMemorySize; //64kb
  static const char WildCardStr[];// "$REPLACE$"
  /**
   * Fragment shader file name
   */
	char* m_fragFileName;
  
  /**
   * Vertex shader file name
   */
  char* m_vertFileName;

  /**
   *	Number of bytes in the constant memory (uniform memory).
   *    Assumes that the max value is defined by the var MaxUniformMemorySize.
   */
  int m_uniformMemorySize; 

  /**
   * Shader uniform variables.
   *  Maps the variable name with the value.
   *  Necessary for the correct reload of the shader.
   */
  map<string, GLint>    m_uniformIntVec;
  map<string, GLfloat>  m_uniformFloatVec;
  map<string, Vector3>  m_uniformVector3Vec;
  map<string, const GLfloat*> m_uniformMatrixVec;

  map<string, pair<GLfloat*, int> > m_uniformFloatArrayVec;
  map<string, pair<GLfloat*, int> > m_uniformFloat2ArrayVec;
  map<string, pair<GLfloat*, int> > m_uniformVector3ArrayVec;

  /**
   *	Define Replacement List
   *    #defines and values used to replace a special wild card word in the shader files.
   *    The wild card word is 
   */
  vector< pair<string, string> > m_defineReplacementList;

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
	GLShader();
  
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
   *	Set Shader FileNames and read them
   */
  void setShaderFiles(char* vert, char* frag);

  /**
   * Activates and deactivates the shader.
   */
	void setActive(bool active);

  /**
   * Set a uniform variable to the shader.
   */
  void setUniformTexture(char* name, GLuint id);
  void setUniformInt(char* name, GLint value);
  void setUniformFloat(char* name, GLfloat value);
  void setUniformVec3(char* name, Vector3 value);
  void setUniformMatrix4(char* name, const GLfloat* value);

  void setUniformFloatArray(char* name, GLfloat *value, int n);
  void setUniformFloat2Array(char* name, GLfloat *value, int n);
  void setUniformVec3Array(char* name, GLfloat *value, int n);

  /**
   *	Define Replacement Functions
   */
  void addReplaceDefine(string defineName, int defineValue);
  void addReplaceDefine(string defineName, bool defineValue);
  void addReplaceDefine(string defineName, float defineValue);
  void addReplaceDefine(string defineName, string defineValue);

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
   *	Replace the defines that contain the wild card WildCardStr
   */
  void replaceDefines(char* shaderText);

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