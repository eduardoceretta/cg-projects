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
  string m_fileName;
public:
  UmMeshFile(void);
  ~UmMeshFile(void);

  virtual void readFile(string fileName,Vector3 pos = Vector3(0,0,0), Vector3 scale = Vector3(1,1,1));
  void writeBinaryFile(string fileName);

protected:
  void calcTriangles();
  virtual void calcVBO();
};