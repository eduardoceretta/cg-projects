/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#pragma once

#include <vector>
#include "MeshLoaders/MeshFileBase.h"

#include "Objects/Triangle.h"

using namespace std;

class MshMeshFile : public MeshFileBase
{
  vector<Triangle> m_triangles;
public:
  MshMeshFile(void);
  ~MshMeshFile(void);

  virtual VertexBufferObject* readFile(unsigned int materialIndex, string fileName, Vector3 pos, Vector3 scale);
  void writeBinaryFile(string fileName);
protected:
   void calcVBO();
   void applyScaleTranslateToVertexes();
   
};
