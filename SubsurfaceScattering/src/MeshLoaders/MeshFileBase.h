/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#pragma once

#include <vector>
#include "GraphBasis/Vector3.h"
#include "GraphBasis/VertexBufferObject.h"
#include "Objects/Triangle.h"

using namespace std;

class MeshFileBase
{
protected:
  string m_fileType;
  Vector3 m_pos;
  Vector3 m_scale;

  VertexBufferObject *m_vbo;

  int m_numVertices;
  int m_numTriangles;
  GLfloat * m_normals;
  GLfloat * m_vertices;
  unsigned int * m_indexes;
public:
  MeshFileBase(void);
  ~MeshFileBase(void);

  virtual bool isValidFileType(string filetype);

  virtual void readFile(string fileName, Vector3 pos = Vector3(0,0,0), Vector3 scale = Vector3(1,1,1)) = 0;
  
  int getNumVertices() const;
  int getNumTriangles() const;

  GLfloat * getVertexes() const;
  GLfloat * getNormals() const;
  unsigned int * getIndexes() const;

  VertexBufferObject * getVbo();


protected:
  virtual void calcVBO() = 0;
};
