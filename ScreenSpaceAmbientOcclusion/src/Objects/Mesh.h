/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#pragma once
#include <string>

#include "GraphBasis/Vector3.h"
#include "MeshLoaders/P3bMeshFile.h"

using namespace std;

class VertexBufferObject;

class Mesh
{
   static int sMeshNum;

   protected:
      VertexBufferObject* mVbo;
      P3bMeshFile * m_p3bMesh;
      bool mCalculated;

      int mMaterialIndex;
      Vector3 mPos;
      Vector3 mScale;

      int myMeshNum;
   public:
      Mesh();
      ~Mesh();

      int getMyMeshNumber() const;
      void readFromStr(char buffer[]);

      int getMaterialIndex();
      VertexBufferObject* getVbo() const;
      P3bMeshFile * getP3bMesh() const;

      void configure();
      void render();
   protected:
};

