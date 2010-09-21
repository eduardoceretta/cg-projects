/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#pragma once

#include <vector>
#include "MeshLoaders/MeshFileBase.h"

#include "Objects/Triangle.h"

using namespace std;

class MsbMeshFile : public MeshFileBase
{
public:
  MsbMeshFile(void);
  ~MsbMeshFile(void);

  virtual VertexBufferObject* readFileToVBO(unsigned int materialIndex, string fileName, Vector3 pos, Vector3 scale);
  virtual void readFileTriangles(string fileName, unsigned int materialIndex = 0, Vector3 pos = Vector3(0,0,0), Vector3 scale = Vector3(1,1,1));
protected:
  
};
