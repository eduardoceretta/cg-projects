/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#pragma once

#include <vector>
#include "GraphBasis/Vector3.h"
#include "GraphBasis/Color.h"

using namespace std;

class Triangle
{
    static unsigned int sTriangleNum;

    unsigned int mMaterialIndex;
    int mMyTriangleNum;
public:
   Vector3 v1, v2, v3;
   Triangle();
   Triangle(unsigned int materialIndex, Vector3 V1, Vector3 V2, Vector3 V3);

   unsigned int getMaterialIndex();
   unsigned int getGlobalIndex();
   static unsigned int getMaxNumTriangles();

   static vector<Triangle> readFromFile(unsigned int materialIndex, string fileName);
};

