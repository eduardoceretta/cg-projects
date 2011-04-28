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
	
  void renderShader();
  void renderOutput(int texIndex = 0);
  void setActive(bool op);
  void setActiveShaderOnly(bool op);
  void step();
	GLuint getOutputTexture(int index);
protected:
  void renderQuad();

	void activateTextures();
	GLuint addInputTexture(GLenum textureDimension, char* name, GLuint id);
	GLuint addInputVec3(char* name, Vector3 value);
	GLuint addInputInt(char* name, GLint value);
  GLuint addInputFloat(char* name, GLfloat value);
  GLuint addInputMatrix4(char* name, const GLfloat* value);
	GLuint addOutput(int index, GLuint textureId = -1);
  GLuint setInputTexture(GLenum textureDimension, char* name, GLuint id, int index);
  GLuint setOutput(int index, GLuint textureId);
  void renderScreenQuad();
  
  FrameBufferObject* m_fbo;
	Shader* m_shader;
  std::vector<std::pair<GLenum, GLuint>> m_inputTextures;
	std::vector<GLuint> m_outputTextures;
};


#endif