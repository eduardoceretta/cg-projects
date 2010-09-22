/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#include <iostream>
#include "main.h"

#include "MeshLoaders/MeshLoader.h"
#include "GraphBasis/VertexBufferObject.h"

#include "MeshLoaders\MshMeshFile.h"
#include "MeshLoaders\MsbMeshFile.h"
#include "MeshLoaders\UmMeshFile.h"

using namespace std;

MeshFileBase* MeshLoader :: sMeshLoaders[] = 
{
  new MshMeshFile(),
  new UmMeshFile(),
  new MsbMeshFile(),
};

///////////////////
//~ MeshLoader
//////////////////
MeshLoader :: MeshLoader()
:MeshFileBase()
,m_meshLoader(NULL)
{
}

void MeshLoader::readFile( string fileName, Vector3 pos /*= Vector3(0,0,0)*/, Vector3 scale /*= Vector3(1,1,1)*/ )
{
  int index = fileName.find_last_of(".");
  MyAssert("Invalid FileName: " + fileName, index!=string::npos);
  string sub = fileName.substr(index, string::npos);

  bool knownFileType = false;
  for(int i=0;i<sizeof(sMeshLoaders)/sizeof(MeshFileBase*); ++i)  
    if(sMeshLoaders[i]->isValidFileType(sub))
    {
      m_meshLoader = sMeshLoaders[i];
      m_meshLoader->readFile(fileName, pos, scale);
      m_numVertices = m_meshLoader->getNumVertices();
      m_numTriangles = m_meshLoader->getNumTriangles();
      m_vertices = m_meshLoader->getVertexes();
      m_normals = m_meshLoader->getNormals();
      m_indexes = m_meshLoader->getIndexes();
      knownFileType = true;
      break;
    }

  MyAssert("Unknown File Type:" + fileName, knownFileType);

}

void MeshLoader::calcVBO()
{
  if(m_meshLoader)
    m_vbo = m_meshLoader->getVbo();
}
