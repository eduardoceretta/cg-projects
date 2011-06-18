/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Mar 2010
 *
 *  OpenGL Mesh Attributes Container
 *  A VBO/P3BMesh with a material and a transformation
 */

#include <iostream>
#include "defines.h"

#include "ScScene/ScMesh.h"
#include "GLUtils/GLVertexBufferObject.h"
#include "MeshLoaders/MeshLoader.h"

using namespace std;

int ScMesh :: s_meshNum = 0;


ScMesh :: ScMesh()
:m_calculated(false)
,m_vbo(NULL)
,m_p3bMesh(NULL)
{
  m_myMeshNum = s_meshNum++;
}

ScMesh :: ~ScMesh()
{
}


int ScMesh :: getMyMeshNumber() const
{
   return m_myMeshNum;
}

void ScMesh :: readFromStr(char buffer[])
{
   int r = sscanf( buffer, "%d %f %f %f %f %f %f %s\n", &m_materialIndex, &m_pos.x, &m_pos.y, &m_pos.z,
      &m_scale.x, &m_scale.y, &m_scale.z, buffer);
   cout << "Ler Arquivo " << buffer <<endl;
   assert(r == 8);

   string fileName(buffer);

   int index = fileName.find_last_of(".");
   MyAssert("Invalid FileName: " + fileName, index!=string::npos);
   string sub = fileName.substr(index, string::npos);

   MeshLoader m;
   if(m.isValidFileType(sub))
   {
     m.readFile(fileName, m_pos, m_scale);
     m_vbo = m.getVbo();
   }else
   {
     if(P3bMeshFile::isValidFileType(sub))
     {
       if(m_p3bMesh)
         delete m_p3bMesh;
       m_p3bMesh = new P3bMeshFile();
       m_p3bMesh->readFile(fileName, m_pos, m_scale);
     }
   }
}


int ScMesh :: getMaterialIndex()
{
   return m_materialIndex;
}


void ScMesh :: configure()
{
}

void ScMesh :: render()
{
  if(m_vbo)
  {
    glPushMatrix();
      glTranslatef(m_pos.x, m_pos.y, m_pos.z);
      m_vbo->render();
    glPopMatrix();
  }

  if(m_p3bMesh)
    m_p3bMesh->render();
}

GLVertexBufferObject* ScMesh::getVbo() const
{
  return m_vbo;
}

P3bMeshFile * ScMesh::getP3bMesh() const
{
  return m_p3bMesh;
}