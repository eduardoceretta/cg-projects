/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#pragma once
#include <string>

#include "GraphBasis/Vector3.h"
#include "MeshLoaders/MeshFileBase.h"

using namespace std;

class MeshLoader: public MeshFileBase
{
 static MeshFileBase* sMeshLoaders[];
 MeshFileBase* m_meshLoader;

 public:
    MeshLoader();
    virtual bool isValidFileType(string filetype);
    void readFile(string fileName, Vector3 pos = Vector3(0,0,0), Vector3 scale = Vector3(1,1,1));
protected:
  void calcVBO();

};

