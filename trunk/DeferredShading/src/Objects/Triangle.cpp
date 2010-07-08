/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/

#include <iostream>
#include <string>




#include "Objects\Triangle.h"
#include <assert.h>

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

vector<Triangle> Triangle :: readFromFile(unsigned int materialIndex, string fileName, Vector3 pos, Vector3 scale)
{
  int index = fileName.find_last_of(".");
  assert(index!=string::npos);
  string sub = fileName.substr(index, string::npos);
  
  if(sub.compare(".um")==0 || sub.compare(".Um")==0 || sub.compare(".uM")==0 || sub.compare(".UM")==0)
    return readUmFile(materialIndex, fileName, pos, scale);
  else if(sub.compare(".msh")==0 || sub.compare(".MSH")==0)
    return readMshFile(materialIndex, fileName,pos, scale);
  
  cout << "Unknown File Type:" << fileName << endl;
  assert(false);

  vector<Triangle> a; 
  return a;
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


vector<Triangle> Triangle::readUmFile(unsigned int materialIndex, string fileName, Vector3 pos, Vector3 scale)
{
  FILE *file;
  file = fopen(fileName.c_str(), "rt");
  assert(file);

  int numVertex, numTriangles;

  fscanf(file, "%d\n", &numVertex);
  printf("Reading %d Vertices...\n", numVertex);

  Vector3 * vList = new Vector3[numVertex];
  Vector3 * nList = new Vector3[numVertex];
  
  for(int i = 0; i < numVertex; ++i)
  {
    nList[i] = Vector3(0,0,0);
    fscanf(file, "%f %f %f\n", &vList[i].x, &vList[i].y, &vList[i].z); 
    
    vList[i].x*=scale.x;
    vList[i].y*=scale.y;
    vList[i].z*=scale.z;

    //vList[i].x+=pos.x;
    //vList[i].y+=pos.y;
    //vList[i].z+=pos.z;
  }

  fscanf(file, "\n%d\n", &numTriangles);
  printf("Reading %d Triangles...\n", numTriangles);

  Vector3 * tList = new Vector3[numTriangles];

  for(int i = 0; i < numTriangles; ++i)
  {
    int t1, t2, t3;
    fscanf(file, "%d %d %d\n", &t1, &t2, &t3);

    nList[t1] += (vList[t2] - vList[t1]) ^ (vList[t3] - vList[t2]);
    nList[t2] += (vList[t2] - vList[t1]) ^ (vList[t3] - vList[t2]);
    nList[t3] += (vList[t2] - vList[t1]) ^ (vList[t3] - vList[t2]);

    tList[i] = Vector3(t1,t2,t3);
  }

  for(int i = 0; i < numVertex; ++i)
  {
    nList[i] = nList[i].unitary();
  }

  vector<Triangle> tVector = vector<Triangle>();
  tVector.reserve(numTriangles);
  for(int i = 0; i < numTriangles; ++i)
  {
    Vector3 t = tList[i];
    int t1 = t.x, t2 = t.y, t3 = t.z;

    Triangle rt(materialIndex, vList[t1], vList[t2], vList[t3], nList[t1], nList[t2], nList[t3] );
    tVector.push_back(rt);
  }

  fclose(file);

  delete[] vList;
  delete[] nList;
  delete[] tList;

  return tVector;
}

vector<Triangle> Triangle::readMshFile(unsigned int materialIndex, string fileName, Vector3 pos, Vector3 scale )
{
  FILE *file;
  file = fopen(fileName.c_str(), "rt");
  assert(file);

  int numVertex, numTriangles;

  fscanf(file, "%d %d\n", &numVertex, &numTriangles);
  printf("Reading %d Vertices...\n", numVertex);

  Vector3 * vList = new Vector3[numVertex];
  Vector3 * nList = new Vector3[numVertex];
  Vector3 * tList = new Vector3[numTriangles];

  for(int i = 0; i < numVertex; ++i)
  {
    nList[i] = Vector3(0,0,0);

    fscanf(file, "%*d %f %f %f\n", &vList[i].x, &vList[i].y, &vList[i].z); 
    //vList[i].x+=pos.x;
    //vList[i].y+=pos.y;
    //vList[i].z+=pos.z;

    vList[i].x*=scale.x;
    vList[i].y*=scale.y;
    vList[i].z*=scale.z;
  }

  printf("Reading %d Triangles...\n", numTriangles);

   for(int i = 0; i < numTriangles; ++i)
  {
    int t1, t2, t3;
    fscanf(file, "%*d %d %d %d\n", &t1, &t2, &t3);
    
    nList[t1] += (vList[t2] - vList[t1]) ^ (vList[t3] - vList[t2]);
    nList[t2] += (vList[t2] - vList[t1]) ^ (vList[t3] - vList[t2]);
    nList[t3] += (vList[t2] - vList[t1]) ^ (vList[t3] - vList[t2]);

    tList[i] = Vector3(t1,t2,t3);
  }

  for(int i = 0; i < numVertex; ++i)
  {
    nList[i] = nList[i].unitary();
  }

  vector<Triangle> tVector = vector<Triangle>();
  tVector.reserve(numTriangles);
  for(int i = 0; i < numTriangles; ++i)
  {
    Vector3 t =  tList[i];
    int t1 = t.x, t2 = t.y, t3 = t.z;
    
    Triangle rt(materialIndex, vList[t1], vList[t2], vList[t3], nList[t1], nList[t2], nList[t3] );
    tVector.push_back(rt);
  }

  fclose(file);

  delete[] vList;
  delete[] nList;
  delete[] tList;

  return tVector;
}


