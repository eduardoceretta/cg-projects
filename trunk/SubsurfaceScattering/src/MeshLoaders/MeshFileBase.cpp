/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#include "MeshLoaders\MeshFileBase.h"
#include <string.h>

MeshFileBase::MeshFileBase(void)
:m_vbo(NULL)
{
}

MeshFileBase::~MeshFileBase(void)
{
}

bool MeshFileBase::isValidFileType( string filetype )
{
  return strcmp(m_fileType.c_str(), strlwr((char*)filetype.c_str())) == 0;
}