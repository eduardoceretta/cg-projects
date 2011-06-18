/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Mar 2011
 *
 *  MSB Model Loader. Imports a binary mesh file that contain only the VBO.
 */
#include "MeshLoaders/MsbMeshFile.h"
#include "defines.h"

MsbMeshFile::MsbMeshFile(void):MeshFileBase()
{
  m_fileType = ".msb";
}

MsbMeshFile::~MsbMeshFile(void)
{
}

void MsbMeshFile::readFile( string fileName, Vector3 pos /*= Vector3(0,0,0)*/, Vector3 scale /*= Vector3(1,1,1)*/ )
{
  m_vbo = NULL;
  m_pos = pos;
  m_scale = scale;
  m_fileName = fileName;
  calcVBO();
}

void MsbMeshFile::calcVBO()
{
  FILE *file;
  file = fopen(m_fileName.c_str(), "rb");
  MyAssert("File Not Found: " + m_fileName, file);

  m_vbo = new GLVertexBufferObject();
  m_vbo->readFromFile(file);

  fclose(file);
  cout << "File " << m_fileName << " read successfully! " <<endl;

  m_vbo->calcVBO();
}

