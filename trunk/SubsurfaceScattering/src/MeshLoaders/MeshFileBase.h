/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#pragma once

#include <vector>
#include "GraphBasis/Vector3.h"
#include "GraphBasis/VertexBufferObject.h"

using namespace std;

class MeshFileBase
{
protected:
  string m_fileType;
  VertexBufferObject *m_vbo;
  Vector3 m_pos;
  Vector3 m_scale;
public:
  MeshFileBase(void);
  ~MeshFileBase(void);

  virtual bool isValidFileType(string filetype);
  virtual VertexBufferObject* readFile(unsigned int materialIndex, string fileName, Vector3 pos, Vector3 scale) = 0;
};
