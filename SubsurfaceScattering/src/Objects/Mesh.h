/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#pragma once
#include <string>

#include "GraphBasis/Vector3.h"

using namespace std;

class VertexBufferObject;
class MeshFileBase;

class Mesh
{
   static int sMeshNum;
   static MeshFileBase* sMeshLoaders[];

   protected:
      VertexBufferObject* mVbo;


      bool mCalculated;

      int mMaterialIndex;
      Vector3 mPos;
      Vector3 mScale;

      int myMeshNum;
   public:
      Mesh();

      int getMyMeshNumber() const;
      void readFromStr(char buffer[]);

      int getMaterialIndex();
      VertexBufferObject* getVbo() const;

      void configure();
      void render();
   protected:
};

