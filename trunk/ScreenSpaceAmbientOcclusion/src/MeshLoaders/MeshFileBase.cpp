/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Mar 2011
 *
 *  Abstract Model Loader. Base class for diverse model files.
 *  Import a model and creates a VBO to represent it.
 *  Calculates the Bounding Box.
 *  Apply simple transformations to the model as scale and translate.
 */
#include <string.h>
#include <limits>

#include "defines.h"
#include "MeshLoaders\MeshFileBase.h"


MeshFileBase::MeshFileBase(void)
:m_vbo(NULL)
,m_fileType("")
,m_fileName("")
,m_pos(Vector3(0, 0, 0))
,m_scale(Vector3(1, 1, 1))
,m_numVertices(0)
,m_numTriangles(0)
,m_vertices(NULL)
,m_normals(NULL)
,m_indexes(NULL)
,m_bb_min(Vector3(numeric_limits<float>::infinity( ), numeric_limits<float>::infinity( ), numeric_limits<float>::infinity( )))
,m_bb_max(Vector3(-numeric_limits<float>::infinity( ),-numeric_limits<float>::infinity( ),-numeric_limits<float>::infinity( )))
{
}

MeshFileBase::~MeshFileBase(void)
{
}

bool MeshFileBase::isValidFileType( string filetype )
{
  return strcmp(m_fileType.c_str(), strlwr((char*)filetype.c_str())) == 0;
}

void MeshFileBase::writeBinaryFile(string fileName)
{
  int index = fileName.find_last_of(".");
  MyAssert("Invalid FileName: " + fileName, index!=string::npos);
  
  string sub = fileName.substr(0, index);

  FILE * fp = fopen((sub+".msb").c_str(),"wb");
  cout << "Writing File: " << sub+".msb" << " ...\n " <<endl;
  
  m_vbo->writeToFile(fp);
  fclose(fp);
}

int MeshFileBase::getNumVertices() const
{
  return m_numVertices;
}

GLfloat * MeshFileBase::getNormals() const
{
  return m_normals;
}

GLfloat * MeshFileBase::getVertexes() const
{
  return m_vertices;
}

unsigned int * MeshFileBase::getIndexes() const
{
  return m_indexes;
}

int MeshFileBase::getNumTriangles() const
{
  return m_numTriangles;
}

GLVertexBufferObject * MeshFileBase::getVbo()
{
  if(!m_vbo)
    calcVBO();
  return m_vbo;
}

bool MeshFileBase::isWriteBinaryFileEnabled() const
{
  return m_writeBinaryFile;
}

void MeshFileBase::setWriteBinaryFileEnabled( bool val )
{
  m_writeBinaryFile = val;
}


