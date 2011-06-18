/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Mar 2011
 *
 *  Abstract Model Loader. Base class for diverse model files.
 *  Import a model and creates a VBO to represent it.
 *  Calculates the Bounding Box.
 *  Apply simple transformations to the model as scale and translate.
 */
#pragma once

#include <vector>
#include <string>
#include "MathUtils/Vector3.h"
#include "GLUtils/GLVertexBufferObject.h"

using namespace std;

class MeshFileBase
{
protected:
  /**
   * Accepted model File Type
   */
  string m_fileType;

  /**
   * File Name to import
   */
  string m_fileName;

  /**
   * Position to translate
   */
  Vector3 m_pos;

  /**
   * Value to scale each vertex of the model
   */
  Vector3 m_scale;

  /**
   * Bounding Box points
   */
  Vector3 m_bb_min;
  Vector3 m_bb_max;

  /**
   * VBO pointer that will be filled 
   */
  GLVertexBufferObject *m_vbo;

  /**
   * Model Number of Vertexes
   */
  int m_numVertices;

  /**
   * Model Number of Triangles
   */
  int m_numTriangles;

  /**
   * Model Normals Array
   */
  GLfloat * m_normals;

  /**
   * Model Vertexes Array
   */
  GLfloat * m_vertices;

  /**
   * Model Indexes Array
   */
  unsigned int * m_indexes;

  
  /**
   * Indicates if the automatic generation of a MSB file is executed after the construction
   *  of the VBO.
   *  Default true
   */
  bool m_writeBinaryFile;

public:
  /**
   * Initialize members
   */
  MeshFileBase(void);
  ~MeshFileBase(void);

  /**
   * Compares the given filetype(".typ") with the loader accepted type.
   *  Return true if is a valid file type
   */
  virtual bool isValidFileType(string filetype);

  /**
   * Read the given file and set the attributes to be applied to the model as translation and scale
   */
  virtual void readFile(string fileName, Vector3 pos = Vector3(0,0,0), Vector3 scale = Vector3(1,1,1)) = 0;

  /**
   * Write a MSB file with the VBO produced.
   */
  virtual void writeBinaryFile(string fileName);

  /**
   * Get the number of Vertexes of the model
   */
  int getNumVertices() const;

  /**
   * Get the number of Triangles of the model
   */
  int getNumTriangles() const;

  /**
   * Get the array of vertexes of the model.
   *  Size equals 3*Number of Vertexes
   */
  GLfloat * getVertexes() const;
  
  /**
   * Get the array of normals of the model.
   *  Size equals 3*Number of Vertexes
   */
  GLfloat * getNormals() const;

  /*
   * Get the array of triangle indexes of the model.
   *  Size equals 3*Number of Triangles
   */
  unsigned int * getIndexes() const;

  /**
   * Get the VBO of the model
   *  If the VBO is outdated calculates it.
   */
  GLVertexBufferObject * getVbo();

  /**
   * Getter and Setter for the automatic generation of a MSB file 
   *  after the construction of the VBO.
   */
  bool isWriteBinaryFileEnabled() const;
  void setWriteBinaryFileEnabled(bool val);

protected:
  /**
   * Calculate the VBO with the read vertexes, normals and indexes.
   *  Writes the binary file automatically if m_writeBinaryFile is true
   */
  virtual void calcVBO() = 0;
};
