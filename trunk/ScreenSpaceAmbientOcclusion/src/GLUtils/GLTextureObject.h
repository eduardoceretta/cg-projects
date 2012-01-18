/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  May 2011
 *
 *  OpenGL Texture information extractor.
 *  Saves to a output image using the external lib FreeImage
 */

#ifndef _GL_TEXTURE_OBJECT_H_
#define _GL_TEXTURE_OBJECT_H_

#include <GL/glut.h>
#include <string>

#include "assert.h"

using namespace std;

class GLTextureObject
{
  /**
   * Texture's buffer. 
   *  GL_FLOAT RGBA. 
   *  Size equals w*[h*]4
   */
  GLfloat *m_fbuffer; 
  
  /**
   * Texture's buffer. 
   *  GL_UNSIGNED_INT RGBA. 
   *  Size equals w*[h*]4
   */
  GLuint *m_uibuffer; 
  /**
   * Texture's OpenGL id.
   */
  GLuint m_id;

  /**
   * Specifies the target texture [GL_TEXTURE_1D, GL_TEXTURE_2D]
   */
  GLenum m_target;

  /**
   * Largest texture that the GL can handle
   */
  static GLint s_max_tex_size;
  /**
   * Largest number of active texture that the GL can handle
   */
  static GLint s_max_tex_image_units;

public:
  /**
   * Output Image Supported File Type.
   *  Could be any supported by FileImage, but only those are granted.
   */
  typedef enum ImageFileType {BMP = 0 , JPG = 1, PNG = 2};

  /**
   * Simple Constructor. Sets id as 0.
   */
  GLTextureObject();

  /**
   * Receives texture id .
   */
  GLTextureObject(GLuint id, GLenum target = GL_TEXTURE_2D);
  

  /**
   * Frees memory.
   */
  ~GLTextureObject();

  /**
   * Static Auxiliary Texture Attributes Getters
   */
  static GLint getMaxTextureImageUnits();
  static GLint getMaxTextureSize();

  /**
   * Create\Set a 1D texture.
   *  InternalFormat: Specifies the number of color components in the texture. [GL_ALPHA, GL_RGB, GL_RGBA, GL_RGBA32F_ARB, GL_RGBA32UI_EXT...]
   *  Format: Specifies the format of the pixel data. [GL_ALPHA, GL_RGB, GL_BGR, GL_RGBA, GL_BGRA, GL_LUMINANCE, GL_LUMINANCE_ALPHA...]
   *  Type: Specifies the data type of the pixel data. [GL_UNSIGNED_BYTE, GL_BYTE, GL_UNSIGNED_INT, GL_INT, GL_FLOAT...]
   *  Data: Specifies a pointer to the image data in memory. 
   */
  GLuint createTexture1D(int width, GLenum internalFormat, GLenum format, GLenum type, const GLvoid * data = 0);
  void setTexture1D(GLuint textureId, int width, GLenum internalFormat, GLenum format, GLenum type, const GLvoid * data = 0 );

  /**
   * Create\Set a 2D texture.
   *  InternalFormat: Specifies the number of color components in the texture. [GL_ALPHA, GL_RGB, GL_RGBA, GL_RGBA32F_ARB, GL_RGBA32UI_EXT...]
   *  Format: Specifies the format of the pixel data. [GL_ALPHA, GL_RGB, GL_BGR, GL_RGBA, GL_BGRA, GL_LUMINANCE, GL_LUMINANCE_ALPHA...]
   *  Type: Specifies the data type of the pixel data. [GL_UNSIGNED_BYTE, GL_BYTE, GL_UNSIGNED_INT, GL_INT, GL_FLOAT...]
   *  Data: Specifies a pointer to the image data in memory. 
   */
  GLuint createTexture2D(int width, int height, GLenum internalFormat, GLenum format, GLenum type, const GLvoid * data = 0);
  void setTexture2D(GLuint textureId, int width, int height, GLenum internalFormat, GLenum format, GLenum type, const GLvoid * data = 0);

  /**
   * Create a 2D texture that is used for FBO Data.
   *  InternalFormat: GL_RGBA32F_ARB
   *  Format: GL_RGBA
   *  Type: GL_FLOAT
   *  Data: 0 
   *  MinFilter, MagFilter: GL_NEAREST
   *  sWrap, tWrap: GL_CLAMP_TO_EDGE
   */
  void createFBODataTexture(int width, int height);

  /**
   * Set Texture Filters
   *  MinFilter: The texture minifying function is used whenever the pixel being textured maps to an area greater than one texture element. 
   *  MagFilter: The texture magnification function is used when the pixel being textured maps to an area less than or equal to one texture element. 
   *  [GL_NEAREST, GL_LINEAR]
   */
  void setFilters(GLfloat minFilter, GLfloat magFilter);

  /**
   * Set Texture Wraps
   *  [GL_CLAMP, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_EDGE, GL_MIRRORED_REPEAT]
   */
  void setWraps(GLfloat sWrap, GLfloat tWrap = GL_CLAMP_TO_EDGE);
  
  /**
   * Sets the texture id and allocates memory space.
   *  If there is memory allocated it is deleted.
   */
  void setId(GLuint id, GLenum target = GL_TEXTURE_2D);

  /**
   * Returns associated id.
   */
  GLuint getId();


  /**
   * Returns texture data.
   *  Copies memory from the GPU.
   */
  GLfloat* read1DTextureFloatData(GLenum format = GL_RGBA);
  GLfloat* read2DTextureFloatData(GLenum format = GL_RGBA);
  
  GLuint* read1DTextureUIntData(GLenum format = GL_RGBA_INTEGER_EXT);
  GLuint* read2DTextureUIntData(GLenum format = GL_RGBA_INTEGER_EXT);
  
  /**
   * Returns texture height.
   *  Copies information from the GPU.
   */
  GLuint readTextureHeight();

  /**
   * Returns texture width.
   *  Copies information from the GPU.
   */
  GLuint readTextureWidth();
  
  /**
   * Returns texture number of colors.
   *  [1 = GL_RED|GL_GREEN|GL_BLUE|GL_ALPHA]
   *  [2 = -]
   *  [3 =  GL_RGB|GL_BGR]
   *  [4 =  GL_RGBA|GL_BGRA]
   *  Copies information from the GPU.
   */
  GLuint readTextureNumColors();

  /**
   * Render the texture in the screen.
   *  Render a screen quad with the texture associated.
   */
  void render2DTexture();

  /**
   * Write the texture to a file.
   *  The image will be RGBA UNSIGNED CHAR 32 bits
   */
  void write2DToFile(string path, string fileName, ImageFileType fileType = PNG);
 

};

#endif


