/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#pragma once
#include <string>

#include "GraphBasis/Vector3.h"
#include "Objects/Triangle.h"

using namespace std;

class VertexBufferObject;

class Mesh
{
   static int sMeshNum;

   protected:
      VertexBufferObject* mVbo;
      bool mCalculed;

      int mMaterialIndex;
      Vector3 mPos;
      Vector3 mPos2;
      vector<Triangle> mTriangles;

      int myMeshNum;
   public:
      Mesh();

      int getMyMeshNumber() const;
      void readFromStr(char buffer[]);

      int getMaterialIndex();
	  std::vector<Triangle>* getTriangles();

      void configure();
      void render();
   protected:
       void calcVBO();
};

