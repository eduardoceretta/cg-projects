/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#pragma once

#include <vector>
#include "MeshLoaders/MeshFileBase.h"

#include "Objects/Triangle.h"

using namespace std;

class UmMeshFile : public MeshFileBase
{
  vector<Triangle> m_triangles;
public:
  UmMeshFile(void);
  ~UmMeshFile(void);

  virtual VertexBufferObject* readFile(unsigned int materialIndex, string fileName, Vector3 pos, Vector3 scale);
  void writeBinaryFile(string fileName);

protected:
  void calcVBO();
  void applyScaleTranslateToVertexes(); 
};
