/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
// Baseado em SGPLYLoader de fabraham@tecgraf.puc-rio.br  em Oct 2004

#pragma once

#include <vector>
#include "GraphBasis/Vector3.h"
#include "MeshLoaders/MeshFileBase.h"

#include "Objects/Triangle.h"

using namespace std;

class AssimpMeshFile : public MeshFileBase
{
  string m_fileName;
  const struct aiScene *m_aiscene;
  Vector3 m_bb_min;
  Vector3 m_bb_max;
public:
  AssimpMeshFile(void);
  ~AssimpMeshFile(void);

  virtual void readFile(string fileName, Vector3 pos = Vector3(0,0,0), Vector3 scale = Vector3(1,1,1));
  bool isValidFileType(string filetype);
  
  void writeBinaryFile(string fileName);
protected:
   virtual void calcVBO();
   void calcTriangles();
   void createVbo(struct aiNode* nd, GLfloat * vList, GLfloat * nList, unsigned int * iList, int &vertexOffset, int &indexOffset, struct aiMatrix4x4* trafo);
   void getModelSizes(struct aiNode* nd, int &nVertices, int &nTriangles);
};


