/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#include <iostream>
#include "main.h"

#include "Objects/Mesh.h"
#include "GraphBasis\VertexBufferObject.h"


using namespace std;

int Mesh :: sMeshNum = 0;

#include "MeshLoaders\MeshFileBase.h"
#include "MeshLoaders\MshMeshFile.h"
#include "MeshLoaders\MsbMeshFile.h"
#include "MeshLoaders\UmMeshFile.h"

MeshFileBase* Mesh :: sMeshLoaders[] = 
{
  new MshMeshFile(),
  new UmMeshFile(),
  new MsbMeshFile(),
};

///////////////////
//~ Mesh
//////////////////
Mesh :: Mesh()
:mCalculated(false)
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
      &mScale.x, &mScale.y, &mScale.z, buffer);
   cout << "Ler Arquivo " << buffer <<endl;
   
   string fileName(buffer);

   int index = fileName.find_last_of(".");
   MyAssert("Invalid FileName: " + fileName, index!=string::npos);
   string sub = fileName.substr(index, string::npos);

   bool knownFileType = false;
   for(int i=0;i<sizeof(sMeshLoaders)/sizeof(MeshFileBase*); ++i)  
     if(sMeshLoaders[i]->isValidFileType(sub))
     {
       mVbo = sMeshLoaders[i]->readFile(mMaterialIndex, fileName, mPos, mScale);
       knownFileType = true;
       break;
     }

   MyAssert("Unknown File Type:" + fileName, knownFileType && mVbo);

   assert(r == 8);

//   cout << "Mesh materialindex, pos, pos2, fileName:\n" << mMaterialIndex <<endl << mPos << mPos2 << buffer <<endl;
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
      mVbo->render();
   glPopMatrix();
}

VertexBufferObject* Mesh::getVbo() const
  {
  return mVbo;
  }