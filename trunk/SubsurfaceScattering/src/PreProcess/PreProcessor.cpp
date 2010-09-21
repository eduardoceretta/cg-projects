/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "main.h"
#include "PreProcess/PreProcessor.h"

using namespace std;

Vector3 g_lightDirection = Vector3(1,1,1);

///////////////////
//~ PreProcessor
//////////////////
PreProcessor::PreProcessor( GLfloat * vertices,GLfloat * normals, int numVertices )
:m_vertices(vertices)
,m_normals(normals)
,m_numVertices(numVertices)
,m_vertexInfo(NULL)
,m_vertexNeighbor(NULL)
{
  m_neighborDistance = .5f;

}

PreProcessor::~PreProcessor()
{
  if(m_vertexInfo)
    delete [] m_vertexInfo;

  if(m_vertexNeighbor)
   delete [] m_vertexNeighbor;
}

float PreProcessor::getNeighborDistance() const
{
  return m_neighborDistance;
}

void PreProcessor::setNeighborDistance( float val )
{
  m_neighborDistance = val;
}

void PreProcessor::calcNeighborhood()
{
  //MUST NOT EXIST REPEATED VETEXES!!!!!!!!!!!!!!
  m_vertexInfo = new VertexInfo[m_numVertices];
  m_vertexNeighbor = new vector<int>[m_numVertices];
  for(int i=0;i<m_numVertices;++i)
  {
    m_vertexInfo[i].index = i;
    m_vertexInfo[i].vertex = Vector3(m_vertices[i*3], m_vertices[i*3+1], m_vertices[i*3+2]);
    m_vertexInfo[i].normal = Vector3(m_normals[i*3], m_normals[i*3+1], m_normals[i*3+2]);
    
    float area = 0;
    for(int j=0;j<m_numVertices;++j)
    {
      if(i==j) continue;
      Vector3 vertex = Vector3(m_vertices[j*3], m_vertices[j*3+1], m_vertices[j*3+2]);
      float d =  ~(m_vertexInfo[i].vertex - vertex);
      if(d < m_neighborDistance)
      {
        m_vertexNeighbor[i].push_back(j);
      }
    }
    m_vertexInfo[i].area = area/3;
  }
}

void PreProcessor::calcLightTerms()
{
  float R;
  float Q; // One Directional light g_lightDirection
  float q; // One Directional light g_lightDirection
  int numNeighbors;
  int neighborFileIndex;
}