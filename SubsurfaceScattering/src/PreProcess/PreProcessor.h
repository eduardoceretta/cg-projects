/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#pragma once

#include <vector>
#include <string>

#include "GraphBasis/Vector3.h"
#include "GraphBasis/VertexBufferObject.h"

using namespace std;

extern Vector3 g_lightDirection;

typedef struct 
{
  Vector3 vertex;
  Vector3 normal;
  int index;
  float R;
  float Q; // One Directional light g_lightDirection
  float q; // One Directional light g_lightDirection
  int numNeighbors;
  int neighborFileIndex;
  float area; // One third of the triangle's area that contains this vertex
}VertexInfo;

class PreProcessor
{
protected:
  GLfloat * m_vertices;
  GLfloat * m_normals;
  int m_numVertices;
  float m_neighborDistance;

  VertexInfo * m_vertexInfo;
  vector<int> * m_vertexNeighbor;

public:
  PreProcessor(GLfloat * vertices, GLfloat * normals, int numVertices);
  ~PreProcessor();

  float getNeighborDistance() const;
  void setNeighborDistance(float val);

protected:
  void calcNeighborhood();
  void calcLightTerms(); // Calc`s R, Q, q
};

