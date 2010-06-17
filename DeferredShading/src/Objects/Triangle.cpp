/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/

#include <iostream>


#include "Objects\Triangle.h"
#include <assert.h>

unsigned int Triangle :: sTriangleNum = 0;

///////////////////
//~ Triangle
//////////////////
Triangle :: Triangle()
{
  sTriangleNum++;
  mMyTriangleNum = sTriangleNum;
}

Triangle :: Triangle(unsigned int MATERIALINDEX, Vector3 V1, Vector3 V2, Vector3 V3)
:mMaterialIndex(MATERIALINDEX)
,v1(V1)
,v2(V2)
,v3(V3)
{
  sTriangleNum++;
  mMyTriangleNum = sTriangleNum;
}

vector<Triangle> Triangle :: readFromFile(unsigned int materialIndex, string fileName)
{
   FILE *file;
   file = fopen(fileName.c_str(), "rt");
   assert(file);

   int numVertex, numTriangles;

   fscanf(file, "%d\n", &numVertex);
   printf("Reading %d Vertices...\n", numVertex);

   Vector3 * vList = new Vector3[numVertex];

   for(int i = 0; i < numVertex; ++i)
      fscanf(file, "%f %f %f\n", &vList[i].x, &vList[i].y, &vList[i].z);

   fscanf(file, "\n%d\n", &numTriangles);
   printf("Reading %d Triangles...\n", numTriangles);

   vector<Triangle> tList = vector<Triangle>();
   tList.reserve(numTriangles);
   for(int i = 0; i < numTriangles; ++i)
   {
      int t1, t2, t3;
      fscanf(file, "%d %d %d\n", &t1, &t2, &t3);
	    Triangle rt(materialIndex, vList[t1], vList[t2], vList[t3]);
      tList.push_back(rt);
   }

   fclose(file);

   delete[] vList;

   return tList;
}

unsigned int Triangle::getGlobalIndex()
{
  return mMyTriangleNum;
}

unsigned int Triangle::getMaxNumTriangles()
{
  return sTriangleNum;
}

unsigned int Triangle::getMaterialIndex()
{
  return mMaterialIndex;
}


