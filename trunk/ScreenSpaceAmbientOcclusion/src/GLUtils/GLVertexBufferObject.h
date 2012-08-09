/**
 *	Eduardo Ceretta Dalla Favera with modifications of Cesar Tadeu Pozzer
 *  eduardo.ceretta@gmail.com, pozzer3@gmail.com
 *  Mar 2010  
 *
 *  OpenGL VertexBufferObject utility class.
 *  Considers 3D Vertexes, 3D Normals and RGB Colors
 */

#pragma once

#include <GL/glut.h>
#include <vector>

#include "MathUtils/Vector3.h"


using namespace std;

/**
 * Auxiliary VBO buffer struct. 
 *  Holds OpenGL memory buffer information.
 */
typedef struct vbobuffer
{



  /**
   * data array
   */
  void* data;

  /**
   * OpenGL memory type.
   *  (GL_FLOAT, GL_BYTE...)
   */
  GLenum type;
  
  /**
   * Number of elements in the VBO.
   */
  int n; 

  /**
   * Memory offset.
   *  see calcVBO()
   */
  int offset; 

  /**
   *	Client State
   *   equals OpenGL client state
   *   For generic vertex attribute there is only 3 for each size(1,2,3,4).
   *   (More will be created as needed).
   */
  enum ClientState 
  {
    VERTEX_ARRAY = GL_VERTEX_ARRAY, //0x8074
    NORMAL_ARRAY = GL_NORMAL_ARRAY, //0x8075
    COLOR_ARRAY = GL_COLOR_ARRAY,   //0x8076
    INDEX_ARRAY = GL_INDEX_ARRAY,   //0x8077
    TEXTURE_COORD_ARRAY = GL_TEXTURE_COORD_ARRAY, //0x8078
    EDGE_FLAG_ARRAY = GL_EDGE_FLAG_ARRAY, //0x8079
    VERTEX_ATTRIB1_ARRAY_0 = 0,

    VERTEX_ATTRIB2_ARRAY_0 = 20,
    
    VERTEX_ATTRIB3_ARRAY_0 = 40,
    VERTEX_ATTRIB3_ARRAY_1,
    VERTEX_ATTRIB3_ARRAY_2,
    VERTEX_ATTRIB3_ARRAY_3,

    VERTEX_ATTRIB4_ARRAY_0 = 60,
  }clientState;

  //GLenum clientState;
  
  /**
   * struct Constructor
   */
  vbobuffer();
  
  /**
   * Returns buffer size in bytes.
   */
  int sizeInBytes();

  
  //chamada na funcao render() do VBO
  /**
   * Sets the respective buffer pointer to the OpenGL.
   *  Considers vertexes 3D
   *  Considers colors RGB
   */
  void setPointer();

  /**
   * Enable\Disable client pointer to the OpenGL Render.
   */
  void enable();
  void disable();
}GLVBOBuffer;


/**
 * OpenGL VertexBufferObject utility class.
 */
class GLVertexBufferObject
{
  /**
   * Indicates if the vertex buffer object is supported by the hardware.
   */
  bool m_supported;
  
  /**
   * Indicates if the associated VBO has an index buffer.
   */
  bool m_hasIndices; 
 
  /**
   * OpenGL VBO ARRAY_BUFFER id.
   */
  GLuint m_vboId;

  /**
   * OpenGL VBO ELEMENT_ARRAY_BUDDER ID.
   *  Exist only if m_hasIndices is true
   */
  GLuint m_vboIndicesId;

  /**
   * OpenGL geometry primitive type.
   *  (GL_TRIANGLES, GL_QUADS, GL_POINTS ...)
   */
  GLenum m_primitive;

  /**
   * Total memory size int bytes.
   */
  int m_VBOBuffersTotalSize;

  /**
   * Indicated the current state of the process.
   *  If false the VBO needs to be updated.
   */
  bool m_calculated;
  
  /**
   * VBO buffers array.
   */
  vector<GLVBOBuffer> m_VBOBuffers;

  /**
   * VBO index buffer.
   *  Exist only if m_hasIndices is true
   */
  GLVBOBuffer m_VBOIndexBuffer;

  /**
   * Bounding Box Info
   */
  Vector3 m_bb_size;
  Vector3 m_bb_center;
  Vector3 m_bb_min;
  Vector3 m_bb_max;
public:
  /**
   * Constructs a VBO of the respective primitive type.
   *  Verifies if the VBO is supported
   */
  GLVertexBufferObject(GLenum primitive = GL_TRIANGLES);
  
  /**
   * Deletes allocated buffers in the GPU's memory
   */
  ~GLVertexBufferObject();

  /**
   * Remove all associated buffers and deletes allocated buffers in the GPU's memory
   */
  void clear();

  /**
   * If the VBO is outdated process the VBO and binds it to the GPU
   */
  void calcVBO();
  
  /**
   * Render the associated VBO
   */
  void render();
  
  /**
   * Returns the OpenGL's geometry primitive type.
   */
  GLenum getPrimitive() const;
  
  /**
   * Sets the OpenGL's geometry primitive type.
   */
  void setPrimitive(GLenum val);

  /**
   * Sets a VBO buffer. 
   *  clientState is the buffer Type (GL_NORMAL_ARRAY, GL_COLOR_ARRAY, GL_VERTEX_ARRAY, (VERTEX_ATTRIB3_ARRAY_0...), GL_TEXTURE_COORD_ARRAY, GL_INDEX_ARRAY)
   *  type is the OpenGL type (GL_FLOAT, GL_BYTE, ...)
   *  n is the number of elements in the buffer.
   *  data is a pointer to the main memory data.
   */
  void setVBOBuffer(vbobuffer::ClientState clientState, GLenum type, int n, void* data);
  void setVBOBuffer(GLenum clientState, GLenum type, int n, void* data);

  /**
   * Sets a VBO index buffer.
   *  type is the OpenGL type (GL_FLOAT, GL_BYTE, ...)
   *  n is the number of elements in the buffer.
   *  data is a pointer to the main memory data.
   */
  void setVBOIndexBuffer(GLenum type, int n, void* data);

  /**
   * Dumps the VBO's data to a file.
   */
  void writeToFile(FILE * fp);

  /**
   * Reads a dumped VBO's data from a file.
   */
  void readFromFile(FILE * fp);


  /**
   * Get the number of elements depending on the type of primitive.
   *  If cannot be calculated return -1
   */
  int getNumElements();

  /**
   * Get the number of vertexes in the array
   */
  int getNumVertexes();

  /**
   * Get Bounding Box info
   */
  Vector3 getBoundingBoxSize() const;
  Vector3 getBoundingBoxCenter() const;
  Vector3 getBoundingBoxMin() const;
  Vector3 getBoundingBoxMax() const;
private:
  /**
   * Verifies if VBO is supported.
   */
  static bool s_IsSupported();

  /**
   * Calculate Bounding Box
   */
  void calcBoundingBox();
};
