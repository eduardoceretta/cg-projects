/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/

#include <iostream>
#include <string>
#include "main.h"

#include "Objects\Triangle.h"


unsigned int Triangle :: sTriangleNum = 0;



///////////////////
//~ Triangle
//////////////////
Triangle :: Triangle()
{
  mMyTriangleNum = sTriangleNum++;
}

Triangle :: Triangle(unsigned int materialIndex, Vector3 V1, Vector3 V2, Vector3 V3, Vector3 N1, Vector3 N2, Vector3 N3)
:mMaterialIndex(materialIndex)
,v1(V1)
,v2(V2)
,v3(V3)
,n1(N1)
,n2(N2)
,n3(N3)
{
  mMyTriangleNum = sTriangleNum++;
}

//vector<Triangle> Triangle :: readFromFile(unsigned int materialIndex, string fileName, Vector3 pos, Vector3 scale)
//{
//  int index = fileName.find_last_of(".");
//  MyAssert("Invalid FileName: " + fileName, index!=string::npos);
//  string sub = fileName.substr(index, string::npos);
//
//  for(int i=0;i<sizeof(sMeshLoaders)/sizeof(MeshFileBase*); ++i)  
//    if(sMeshLoaders[i]->isValidFileType(sub))
//      return sMeshLoaders[i]->readFile(materialIndex, fileName, pos, scale);
//    
//  MyAssert("Unknown File Type:" + fileName, false);
//
//  vector<Triangle> a; 
//  return a;
//}

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


