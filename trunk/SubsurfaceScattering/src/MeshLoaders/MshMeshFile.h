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
  string m_fileName;
public:
  MshMeshFile(void);
  ~MshMeshFile(void);

  virtual void readFile(string fileName, Vector3 pos = Vector3(0,0,0), Vector3 scale = Vector3(1,1,1));
  
  void writeBinaryFile(string fileName);
protected:
   virtual void calcVBO();
   void calcTriangles();
};

