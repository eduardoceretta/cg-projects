#ifndef KernelBase_H
#define KernelBase_H

#include <vector>
#include <utility>

#include <GraphBasis/FrameBufferObject.h>
#include <GraphBasis/Vector3.h>
#include <GraphBasis/Shader.h>

class KernelBase {

public:
	KernelBase();
	KernelBase(char* vert, char* frag, int width, int height);
	~KernelBase();
	
  void reloadShader();
  void renderShader();
  void renderOutput(int texIndex = 0);
  void setActive(bool op);
  void setActiveShaderOnly(bool op);
  void step();
	GLuint getOutputTexture(int index);
protected:
  void renderQuad();

	void activateTextures();
  
	void addInputTexture(GLenum textureDimension, char* name, GLuint id);
	void addInputVec3(char* name, Vector3 value);
	void addInputInt(char* name, GLint value);
  void addInputFloat(char* name, GLfloat value);
  void addInputMatrix4(char* name, const GLfloat* value);

  void setInputTexture(GLenum textureDimension, char* name, GLuint id, int index);

	GLuint addOutput(int index, GLuint textureId = -1);

  GLuint setOutput(int index, GLuint textureId);
  void renderScreenQuad();
  
  FrameBufferObject* m_fbo;
	Shader* m_shader;
  std::vector<std::pair<GLenum, GLuint>> m_inputTextures;
	std::vector<GLuint> m_outputTextures;
};


#endif