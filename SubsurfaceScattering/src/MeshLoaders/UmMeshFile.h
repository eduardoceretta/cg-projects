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
public:
  UmMeshFile(void);
  ~UmMeshFile(void);

  virtual void readFileTriangles(string fileName, unsigned int materialIndex = 0, Vector3 pos = Vector3(0,0,0), Vector3 scale = Vector3(1,1,1));
  virtual VertexBufferObject* readFileToVBO(unsigned int materialIndex, string fileName, Vector3 pos, Vector3 scale);
  void writeBinaryFile(string fileName);

protected:
  void calcTriangles(string fileName);
  void calcTrianglesArrays();
  void calcVBO();
};
