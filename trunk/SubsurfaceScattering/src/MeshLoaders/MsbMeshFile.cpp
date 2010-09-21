/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#include "MeshLoaders/MsbMeshFile.h"
#include <string>
#include "main.h"

MsbMeshFile::MsbMeshFile(void):MeshFileBase()
{
  m_fileType = ".msb";
}

MsbMeshFile::~MsbMeshFile(void)
{
}

VertexBufferObject* MsbMeshFile::readFileToVBO( unsigned int materialIndex, string fileName, Vector3 pos, Vector3 scale )
{
  m_pos = pos;
  m_scale = scale;
  
  FILE *file;
  file = fopen(fileName.c_str(), "rb");
  MyAssert("File Not Found: " + fileName, file);
  
  m_vbo = new VertexBufferObject();
  m_vbo->readFromFile(file);

  fclose(file);
  cout << "File " << fileName << " read successfully! " <<endl;
 
  m_vbo->calcVBO();

  return m_vbo;
}

void MsbMeshFile::readFileTriangles( string fileName, unsigned int materialIndex /*= 0*/, Vector3 pos /*= Vector3(0,0,0)*/, Vector3 scale /*= Vector3(1,1,1)*/ )
{
  readFileToVBO( materialIndex, fileName, pos, scale );
  cout << "Info From VBO(msb file) Not Implemented!" << endl;
}