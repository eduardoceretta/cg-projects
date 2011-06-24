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
   *  Size equals w*h*4
   */
  GLfloat *m_buffer; 
  /**
   * Texture's OpenGL id.
   */
  GLuint m_id;

public:
  /**
   * Output Image Supported File Type.
   *  Could be any supported by FileImage, but only those are garanted.
   */
  typedef enum ImageFileType {BMP = 0 , JPG = 1, PNG = 2};

  /**
   * Simple Constructor. Sets id as 0.
   */
  GLTextureObject();

  /**
   * Receives texture id and allocates memory space.
   */
  GLTextureObject(GLuint id);
  

  /**
   * Frees memory.
   */
  ~GLTextureObject();
  
  /**
   * Sets the texture id and allocates memory space.
   *  If there is memory allocated it is deleted.
   */
  void setId(GLuint id);

  /**
   * Returns associated id.
   */
  GLuint getId();


  /**
   * Returns texture data.
   *  Copies memory from the GPU.
   */
  GLfloat* getTextureData();
  
  /**
   * Returns texture height.
   *  Copies information from the GPU.
   */
  GLuint getTextureHeight();

  /**
   * Returns texture width.
   *  Copies information from the GPU.
   */
  GLuint getTextureWidth();

  /**
   * Render the texture in the screen.
   *  Render a screen quad with the texture associated.
   */
  void renderTexture();

  /**
   * Write the texture to a file.
   *  The image will be RGBA UNSIGNED CHAR 32 bits
   */
  void writeToFile(string fileName, ImageFileType fileType = PNG);
  
};

#endif


