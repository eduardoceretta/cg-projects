/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#include <string.h>
#include "MeshLoaders\MeshFileBase.h"


MeshFileBase::MeshFileBase(void)
:m_vbo(NULL)
,m_fileType("")
,m_pos(Vector3(0, 0, 0))
,m_scale(Vector3(1, 1, 1))
,m_numVertices(0)
,m_numTriangles(0)
,m_vertices(NULL)
,m_normals(NULL)
,m_indexes(NULL)
{
}

MeshFileBase::~MeshFileBase(void)
{
}

bool MeshFileBase::isValidFileType( string filetype )
{
  return strcmp(m_fileType.c_str(), strlwr((char*)filetype.c_str())) == 0;
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

VertexBufferObject * MeshFileBase::getVbo()
{
  if(!m_vbo)
    calcVBO();
  return m_vbo;
}