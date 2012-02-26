/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Mar 2010
 *
 *  OpenGL Mesh Attributes Container
 *  A VBO/P3BMesh with a material and a transformation
 */

#include <iostream>
#include <limits>
#include "defines.h"

#include "ScScene/ScMesh.h"
#include "GLUtils/GLVertexBufferObject.h"
#include "MeshLoaders/MeshLoader.h"

using namespace std;

int ScMesh :: s_meshNum = 0;


ScMesh :: ScMesh()
:m_vbo(NULL)
,m_p3bMesh(NULL)
,m_numVertices(0u)
,m_numElements(0u)
,m_fileName("")
,m_fullFilePath("")
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
   cout << "Reading Model: " << buffer << " ...\n"<<endl;
   assert(r == 8);

   string fileName(buffer);

   int index = fileName.find_last_of(".");
   int indexSlash = fileName.find_last_of("/");
   MyAssert("Invalid FileName: " + fileName, index!=string::npos);
   string sub = fileName.substr(index, string::npos);

   m_fileName = fileName.substr(indexSlash + 1, index - indexSlash - 1);
   m_fullFilePath = fileName;

   MeshLoader m;
   if(m.isValidFileType(sub))
   {
     m.readFile(fileName, m_pos, m_scale);
     m_vbo = m.getVbo();
     
     m_numVertices = m_vbo->getNumVertexes();
     m_numElements = m_vbo->getNumElements();

   }else
   {
     if(P3bMeshFile::isValidFileType(sub))
     {
       if(m_p3bMesh)
         delete m_p3bMesh;
       m_p3bMesh = new P3bMeshFile();
       m_p3bMesh->readFile(fileName, m_pos, m_scale);
      
       m_numVertices = 0;
       m_numElements = 0;
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

Vector3 ScMesh::getBoundingBoxSize() const
{
  Vector3 bb_max = getBoundingBoxMax();
  Vector3 bb_min = getBoundingBoxMin();
  return bb_max - bb_min;
}

Vector3 ScMesh::getBoundingBoxCenter() const
{
  Vector3 bb_max = getBoundingBoxMax();
  Vector3 bb_min = getBoundingBoxMin();
  return (bb_max + bb_min)*.5;
}

Vector3 ScMesh::getBoundingBoxMin() const
{
  if(m_vbo && m_p3bMesh)
  {
    Vector3 v_bb = m_vbo->getBoundingBoxMin();
    Vector3 p_bb = m_p3bMesh->getBoundingBoxMin();
    return m_pos + Vector3(min(v_bb.x, p_bb.x), 
                   min(v_bb.y, p_bb.y), 
                   min(v_bb.z, p_bb.z));
  }

  if(m_vbo)
    return  m_pos + m_vbo->getBoundingBoxMin();

  if(m_p3bMesh)
  {
    Vector3 bb_min =  m_p3bMesh->getBoundingBoxMin();
    bb_min.x *= m_scale.x;
    bb_min.y *= m_scale.y;
    bb_min.z *= m_scale.z;
    return  m_pos + bb_min;
  }

  return Vector3(-numeric_limits<float>::infinity( ), -numeric_limits<float>::infinity( ), -numeric_limits<float>::infinity( ));
}

Vector3 ScMesh::getBoundingBoxMax() const
{
  if(m_vbo && m_p3bMesh)
  {
    Vector3 v_bb = m_vbo->getBoundingBoxMax();
    Vector3 p_bb = m_p3bMesh->getBoundingBoxMax();
    return  m_pos + Vector3(max(v_bb.x, p_bb.x), 
                   max(v_bb.y, p_bb.y), 
                   max(v_bb.z, p_bb.z));
  }

  if(m_vbo)
    return  m_pos + m_vbo->getBoundingBoxMax();

  if(m_p3bMesh)
  {
    Vector3 bb_max =  m_p3bMesh->getBoundingBoxMax();
    bb_max.x *= m_scale.x;
    bb_max.y *= m_scale.y;
    bb_max.z *= m_scale.z;
    return  m_pos + bb_max;
  }

  return Vector3(numeric_limits<float>::infinity( ), numeric_limits<float>::infinity( ), numeric_limits<float>::infinity( ));
}

string ScMesh::getFileName() const
{
  return m_fileName;
}

string ScMesh::getFullFilePath() const
{
  return m_fullFilePath;
}

unsigned int ScMesh::getNumVertices() const
{
  return m_numVertices;
}

unsigned int ScMesh::getNumElements() const
{
  return m_numElements;
}

