/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#include <iostream>
#include "main.h"

#include "Objects/Mesh.h"
#include "GraphBasis\VertexBufferObject.h"
#include "MeshLoaders\MeshLoader.h"

using namespace std;

int Mesh :: sMeshNum = 0;


///////////////////
//~ Mesh
//////////////////
Mesh :: Mesh()
:mCalculated(false)
,mVbo(NULL)
,m_p3bMesh(NULL)
{
   myMeshNum = sMeshNum++;
}

Mesh :: ~Mesh()
{
  //if(m_p3bMesh)
  //  delete m_p3bMesh;
}


int Mesh :: getMyMeshNumber() const
{
   return myMeshNum;
}

void Mesh :: readFromStr(char buffer[])
{
   int r = sscanf( buffer, "%d %f %f %f %f %f %f %s\n", &mMaterialIndex, &mPos.x, &mPos.y, &mPos.z,
      &mScale.x, &mScale.y, &mScale.z, buffer);
   cout << "Ler Arquivo " << buffer <<endl;
   assert(r == 8);
   //   cout << "Mesh materialindex, pos, pos2, fileName:\n" << mMaterialIndex <<endl << mPos << mPos2 << buffer <<endl;

   string fileName(buffer);

   int index = fileName.find_last_of(".");
   MyAssert("Invalid FileName: " + fileName, index!=string::npos);
   string sub = fileName.substr(index, string::npos);

   MeshLoader m;
   if(m.isValidFileType(sub))
   {
     m.readFile(fileName, mPos, mScale);
     mVbo = m.getVbo();
   }else
   {
     if(m_p3bMesh)
       delete m_p3bMesh;
     m_p3bMesh = new P3bMeshFile();

     if(m_p3bMesh->isValidFileType(sub))
     {
       m_p3bMesh->readFile(fileName, mPos, mScale);
     }
   }
}


int Mesh :: getMaterialIndex()
{
   return mMaterialIndex;
}


void Mesh :: configure()
{
   //if(!mCalculated)
   //{

      //calcVBO();
      //FILE * fp = fopen("./resources/Models/teste1.msb","wb");
      //mVbo->writeToFile(fp);
      //fclose(fp);
      //cout << "File " << "./resources/Models/teste1.msb" << " write successfully! " <<endl;
      //applyScaleTranslateToVertexes();

   //   mCalculated = true;
   //}
}

void Mesh :: render()
{
   glPushMatrix();
      glTranslatef(mPos.x, mPos.y, mPos.z);
      if(mVbo)
        mVbo->render();
  glPopMatrix();

  if(m_p3bMesh)
    m_p3bMesh->render();
}

VertexBufferObject* Mesh::getVbo() const
{
  return mVbo;
}

P3bMeshFile * Mesh::getP3bMesh() const
{
  return m_p3bMesh;
}