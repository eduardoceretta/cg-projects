/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#include "MeshLoaders\MeshFileBase.h"
#include <string.h>

MeshFileBase::MeshFileBase(void)
:m_vbo(NULL)
,m_fileType("")
,m_materialIndex(0)
,m_normals(NULL)
,m_numVertices(0)
,m_scale(Vector3(1, 1, 1))
,m_vertices(NULL)
,m_pos(Vector3(0, 0, 0))
{
}

MeshFileBase::~MeshFileBase(void)
{
}

bool MeshFileBase::isValidFileType( string filetype )
{
  return strcmp(m_fileType.c_str(), strlwr((char*)filetype.c_str())) == 0;
}

vector<Triangle> MeshFileBase::Triangles() const
{
  return m_triangles;
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