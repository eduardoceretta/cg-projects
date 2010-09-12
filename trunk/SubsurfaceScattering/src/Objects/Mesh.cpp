/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#include <iostream>

#include "Objects/Mesh.h"
#include "GraphBasis\VertexBufferObject.h"

using namespace std;

int Mesh :: sMeshNum = 0;

///////////////////
//~ Mesh
//////////////////
Mesh :: Mesh()
:mCalculed(false)
,mVbo(NULL)
{
   myMeshNum = sMeshNum++;
}


int Mesh :: getMyMeshNumber() const
{
   return myMeshNum;
}

void Mesh :: readFromStr(char buffer[])
{
   int r = sscanf( buffer, "%d %f %f %f %f %f %f %s\n", &mMaterialIndex, &mPos.x, &mPos.y, &mPos.z,
      &mPos2.x, &mPos2.y, &mPos2.z, buffer);
   cout << "Ler Arquivo " << buffer <<endl;
   mTriangles = Triangle::readFromFile(mMaterialIndex, string(buffer), mPos, mPos2);

   assert(r == 8);

//   cout << "Mesh materialindex, pos, pos2, fileName:\n" << mMaterialIndex <<endl << mPos << mPos2 << buffer <<endl;
}


int Mesh :: getMaterialIndex()
{
   return mMaterialIndex;
}

std::vector<Triangle>* Mesh::getTriangles(){
	return &mTriangles;
}


void Mesh :: configure()
{
   if(!mCalculed)
   {
      calcVBO();
      applyScaleTranslateToVertexes();
      mCalculed = true;
   }
}

void Mesh :: render()
{
   glPushMatrix();
      glTranslatef(mPos.x, mPos.y, mPos.z);
      mVbo->render();
   glPopMatrix();
}


void Mesh :: calcVBO()
{
   mVbo = new VertexBufferObject();
   int n = mTriangles.size()*3;
   GLfloat* vertices = new GLfloat[n*3];
   GLfloat* normals = new GLfloat[n*3];

   for(int i = 0; i < n/3; ++i)
   {
      //Vector3 n = (mTriangles[i].v2 - mTriangles[i].v1) ^ (mTriangles[i].v3 - mTriangles[i].v2);
      //n = n.unitary();

      int index = i*9;
      normals[index] = mTriangles[i].n1.x;
      vertices[index++] = mTriangles[i].v1.x;
      normals[index] = mTriangles[i].n1.y;
      vertices[index++] = mTriangles[i].v1.y;
      normals[index] = mTriangles[i].n1.z;
      vertices[index++] = mTriangles[i].v1.z;
      normals[index] = mTriangles[i].n2.x;
      vertices[index++] = mTriangles[i].v2.x;
      normals[index] = mTriangles[i].n2.y;
      vertices[index++] = mTriangles[i].v2.y;
      normals[index] = mTriangles[i].n2.z;
      vertices[index++] = mTriangles[i].v2.z;
      normals[index] = mTriangles[i].n3.x;
      vertices[index++] = mTriangles[i].v3.x;
      normals[index] = mTriangles[i].n3.y;
      vertices[index++] = mTriangles[i].v3.y;
      normals[index] = mTriangles[i].n3.z;
      vertices[index++] = mTriangles[i].v3.z;
   }

   mVbo->setVBOBuffer( GL_VERTEX_ARRAY, GL_FLOAT, n, vertices);
   mVbo->setVBOBuffer( GL_NORMAL_ARRAY, GL_FLOAT, n, normals);
   mVbo->calcVBO();
}


void Mesh::applyScaleTranslateToVertexes() 
{
  vector<Triangle> :: iterator tIt;
  for(tIt = mTriangles.begin(); tIt != mTriangles.end(); ++tIt){
    //tIt->v1.x *= mScale.x;
    //tIt->v1.y *= mScale.y;
    //tIt->v1.z *= mScale.z;

    //tIt->v2.x *= mScale.x;
    //tIt->v2.y *= mScale.y;
    //tIt->v2.z *= mScale.z;

    //tIt->v3.x *= mScale.x;
    //tIt->v3.y *= mScale.y;
    //tIt->v3.z *= mScale.z;

    tIt->v1.x += mPos.x;
    tIt->v1.y += mPos.y;
    tIt->v1.z += mPos.z;

    tIt->v2.x += mPos.x;
    tIt->v2.y += mPos.y;
    tIt->v2.z += mPos.z;

    tIt->v3.x += mPos.x;
    tIt->v3.y += mPos.y;
    tIt->v3.z += mPos.z;
  }
}
