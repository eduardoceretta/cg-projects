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

public:
  MshMeshFile(void);
  ~MshMeshFile(void);

  virtual VertexBufferObject* readFileToVBO(unsigned int materialIndex, string fileName, Vector3 pos, Vector3 scale);
  virtual void readFileTriangles(string fileName, unsigned int materialIndex = 0, Vector3 pos = Vector3(0,0,0), Vector3 scale = Vector3(1,1,1));
  
  void writeBinaryFile(string fileName);
protected:
   void calcVBO();
   void calcTriangles(string fileName);
   void calcTrianglesArrays();
};

