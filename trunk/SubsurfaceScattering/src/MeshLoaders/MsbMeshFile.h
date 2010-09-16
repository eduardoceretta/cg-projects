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

  virtual VertexBufferObject* readFile(unsigned int materialIndex, string fileName, Vector3 pos, Vector3 scale);
protected:
  
};
