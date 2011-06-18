/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Mar 2011
 *
 *  Generic Model Loader. Imports all the supported model files.
 *  Try to use all the mesh loaders implemented to open the file given.
 */
#include <iostream>
#include "defines.h"

#include "MeshLoaders/MeshLoader.h"
#include "GLUtils/GLVertexBufferObject.h"

#include "MeshLoaders\MshMeshFile.h"
#include "MeshLoaders\MsbMeshFile.h"
#include "MeshLoaders\UmMeshFile.h"
#include "MeshLoaders\PlyMeshFile.h"
#include "MeshLoaders\AssimpMeshFile.h"

using namespace std;
/**
 * Instances of the implemented meshloaders
 */
MeshFileBase* MeshLoader :: s_meshLoaders[] = 
{
  new MshMeshFile(),
  new UmMeshFile(),
  new MsbMeshFile(),
  new PlyMeshFile(),
  new AssimpMeshFile(),
};


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
  for(int i=0;i<sizeof(s_meshLoaders)/sizeof(MeshFileBase*); ++i)  
    if(s_meshLoaders[i]->isValidFileType(sub))
    {
      m_meshLoader = s_meshLoaders[i];
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

bool MeshLoader:: isValidFileType(string filetype)
{
  for(int i=0;i<sizeof(s_meshLoaders)/sizeof(MeshFileBase*); ++i)  
    if(s_meshLoaders[i]->isValidFileType(filetype))
      return true;
  return false;
}

void MeshLoader::calcVBO()
{
  if(m_meshLoader)
    m_vbo = m_meshLoader->getVbo();
}
