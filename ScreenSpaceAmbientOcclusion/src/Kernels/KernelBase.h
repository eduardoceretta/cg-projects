/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Oct 2010  
 *
 *  Base Class for rendering kernels definition.
 *  A kernel is composed by a FBO, a shader and input and output textures.
 */
#ifndef _KERNELBASE_H_
#define _KERNELBASE_H_

#include <vector>
#include <utility>

#include <MathUtils/Vector3.h>

#include "GLUtils/GLFrameBufferObject.h"
#include "GLUtils/GLShader.h"

class KernelBase {
public:
  /**
   * Simple Constructor
   */
	KernelBase();

  /**
   * Creates a FBO and a Shader with the filenames received.
   */
	KernelBase(char* path, char* vert, char* frag, int width, int height);

  /**
   * Creates a FBO and a Shader with the filenames received.
   */
	KernelBase(char* vert, char* frag, int width, int height);

  /**
   * Delete Memory and destroy Shader and FBO
   */
	~KernelBase();
	
  /**
   * Reload the Kernel's Shader
   */
  void reloadShader();

  /**
   * Render the shader in a Screen sized Quad with the input textures bound
   */
  void renderShader();

  /**
  * Render the respective output texture.
  */
  void renderOutput(int texIndex = 0);

  /**
   * Activate/Deactivate the Kernel's FBO and shader
   */
  virtual void setActive(bool op);

  /**
   * Activate/Deactivate the Kernel's shader
   */
  virtual void setActiveShaderOnly(bool op);

  /**
   * Does a kernel pass cycle.
   *  Activate the FBO and shader, bind the input texture, draw a screen quad
   *  and deactivate the shader and the FBO.
   */
  virtual void step();

  /**
   * Return the Texture ID of the respective index.
   */
	GLuint getOutputTexture(int index);

protected:
  /**
   * Render a ScreenQuad With the Input textures bound
   */
  void renderScreenQuad();

	/**
	 * Bind the input textures
	 */
  void activateTextures();
  
  /**
   * Add a Shader Uniform Variable
   */
	void addInputVec3(char* name, Vector3 value);
	void addInputInt(char* name, GLint value);
  void addInputFloat(char* name, GLfloat value);
  void addInputMatrix4(char* name, const GLfloat* value);
  
  /**
   * Add a Shader Uniform Variable Array. 
   *  n is the number of elements of the array.(value size  =  3*n)
   */
  void addInputVec3Array(char* name, GLfloat *value, int n);

  /**
   * Create an Input Texture and passes it to the shader
   */
  void addInputTexture(GLenum textureDimension, char* name, GLuint id);
  
  /**
   * Change the value of Shader texture but keep the same texture channel 
   */
  void setInputTexture(GLenum textureDimension, char* name, GLuint id, int index);

  /**
   * Add an Output Texture. If textureID is -1 a new texture is created
   */
	GLuint addOutput(int index, GLuint textureId = -1);

  /**
   * Set an Output Texture
   */
  GLuint setOutput(int index, GLuint textureId);
 
  /**
   * Frame Buffer used for rendering
   */
  GLFrameBufferObject* m_fbo;

  /**
   * Shader used for rendering
   */
	GLShader* m_shader;

  /**
   * Array of input textures.
   *  Pair of Texture Target (1D, 2D, 3D) and Texture ID
   */
  std::vector<std::pair<GLenum, GLuint>> m_inputTextures;

  /**
   * Array of output 2D textures representing the render targets.
   */
	std::vector<GLuint> m_outputTextures;
};


#endif