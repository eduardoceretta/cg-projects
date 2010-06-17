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
   mTriangles = Triangle::readFromFile(mMaterialIndex, string(buffer));

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
      Vector3 n = (mTriangles[i].v2 - mTriangles[i].v1) ^ (mTriangles[i].v3 - mTriangles[i].v2);
      n = n.unitary();

      int index = i*9;
      normals[index] = n.x;
      vertices[index++] = mTriangles[i].v1.x;
      normals[index] = n.y;
      vertices[index++] = mTriangles[i].v1.y;
      normals[index] = n.z;
      vertices[index++] = mTriangles[i].v1.z;
      normals[index] = n.x;
      vertices[index++] = mTriangles[i].v2.x;
      normals[index] = n.y;
      vertices[index++] = mTriangles[i].v2.y;
      normals[index] = n.z;
      vertices[index++] = mTriangles[i].v2.z;
      normals[index] = n.x;
      vertices[index++] = mTriangles[i].v3.x;
      normals[index] = n.y;
      vertices[index++] = mTriangles[i].v3.y;
      normals[index] = n.z;
      vertices[index++] = mTriangles[i].v3.z;
   }

   mVbo->setVBOBuffer( GL_VERTEX_ARRAY, GL_FLOAT, n, vertices);
   mVbo->setVBOBuffer( GL_NORMAL_ARRAY, GL_FLOAT, n, normals);
   mVbo->calcVBO();
}
