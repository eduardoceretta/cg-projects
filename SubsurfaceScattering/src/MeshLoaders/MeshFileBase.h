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
  int m_materialIndex;

  VertexBufferObject *m_vbo;
  vector<Triangle> m_triangles;

  int m_numVertices;

  GLfloat * m_normals;
  GLfloat * m_vertices;

public:
  MeshFileBase(void);
  ~MeshFileBase(void);

  virtual bool isValidFileType(string filetype);

  virtual void readFileTriangles(string fileName, unsigned int materialIndex = 0, Vector3 pos = Vector3(0,0,0), Vector3 scale = Vector3(1,1,1)) = 0;
  virtual VertexBufferObject* readFileToVBO(unsigned int materialIndex, string fileName, Vector3 pos, Vector3 scale) = 0;
  
  int getNumVertices() const;
  GLfloat * getVertexes() const;
  GLfloat * getNormals() const;

  vector<Triangle> Triangles() const;
};
