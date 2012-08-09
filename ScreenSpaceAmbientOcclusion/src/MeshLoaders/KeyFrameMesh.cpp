/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Aug 2012
 *
 *  Animation Model Loader. Imports diverse file types.
 *  Receives another KeyFrame and then constructs the VBO with the info
 *  needed by the animation interpolation.
 *
 */

#include "MeshLoaders/KeyFrameMesh.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include "defines.h"

#include "MeshLoaders/MeshLoader.h"
#include "GLUtils/GLVertexBufferObject.h"

KeyFrameMesh::KeyFrameMesh(void)
:MeshFileBase()
,m_meshLoader(NULL)
,m_prevKF(NULL)
,m_nextKF(NULL)
{
  m_writeBinaryFile = true;
  m_meshLoader = new MeshLoader();
  m_meshLoader->setWriteBinaryFileEnabled(m_writeBinaryFile);
  m_fileType = "NOFILE";
}

KeyFrameMesh::~KeyFrameMesh(void)
{
  if(m_meshLoader)
    delete m_meshLoader;
}

void KeyFrameMesh::readFile( string fileName, Vector3 pos /*= Vector3(0,0,0)*/, Vector3 scale /*= Vector3(1,1,1)*/ )
{
  m_meshLoader->readFile(fileName, pos, scale);
  m_vertices = m_meshLoader->getVertices();
  m_normals = m_meshLoader->getNormals();
  m_indexes = m_meshLoader->getIndexes();
  m_numVertices = m_meshLoader->getNumVertices();
  m_numTriangles = m_meshLoader->getNumTriangles();
  m_fileName = m_meshLoader->getFileName();
  if(m_vertices == NULL)
    m_vbo = m_meshLoader->getVbo();
  m_pos = pos;
  m_scale = scale;
}

bool KeyFrameMesh::isValidFileType(string filetype)
{
  return m_meshLoader->isValidFileType(filetype);
}


void KeyFrameMesh::setPreviousKeyFrame( MeshFileBase *p )
{
  if(p != NULL)
  {
    if(p->getNumVertices() == m_numVertices && p->getNumTriangles() == m_numTriangles)
      m_prevKF = p;
  }else 
    m_prevKF = NULL;
}

void KeyFrameMesh::setNextKeyFrame( MeshFileBase *n )
{
  if(n != NULL)
  {
    if(n->getNumVertices() == m_numVertices && n->getNumTriangles() == m_numTriangles)
      m_nextKF = n;
  }else 
    m_nextKF = NULL;
}


void KeyFrameMesh::calcVBO()
{
  m_vbo = new GLVertexBufferObject();
  m_vbo->setVBOBuffer(GL_VERTEX_ARRAY, GL_FLOAT, m_numVertices, m_vertices);
  m_vbo->setVBOBuffer(GL_NORMAL_ARRAY, GL_FLOAT, m_numVertices, m_normals);
  m_vbo->setVBOIndexBuffer(GL_UNSIGNED_INT, m_numTriangles*3, m_indexes);
  //if(m_nextKF)
  //{
  //  //Vertices
  //  m_vbo->setVBOBuffer(vbobuffer::VERTEX_ATTRIB3_ARRAY_0, GL_FLOAT, m_numVertices, m_nextKF->getVertices());
  //  //Normals
  //  m_vbo->setVBOBuffer(vbobuffer::VERTEX_ATTRIB3_ARRAY_1, GL_FLOAT, m_numVertices, m_nextKF->getNormals());
  //}

  //if(m_prevKF)
  //{
  //  //Vertices
  //  m_vbo->setVBOBuffer(vbobuffer::VERTEX_ATTRIB3_ARRAY_2, GL_FLOAT, m_numVertices, m_prevKF->getVertices());
  //  //Normals
  //  m_vbo->setVBOBuffer(vbobuffer::VERTEX_ATTRIB3_ARRAY_3, GL_FLOAT, m_numVertices, m_prevKF->getNormals());
  //}


  m_vbo->calcVBO();
  if(m_writeBinaryFile)
    writeBinaryFile(m_fileName);
}

