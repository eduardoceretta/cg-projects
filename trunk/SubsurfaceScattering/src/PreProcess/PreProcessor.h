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
  //int index;
  float indexF;
  Vector3 vertex;
  Vector3 normal;

  float area; // One third of the triangle's area that contains this vertex

  //int numNeighbors;
  float numNeighborsF;
  

  //int neighborFileIndex;
  float neighborFileIndexF;

  //int RFileIndex;
  float RFileIndexF;
  Vector3 Q; // One Directional light g_lightDirection
  float q; // One Directional light g_lightDirection
}VertexInfo;

class PreProcessor
{
protected:
  //INPUT
  GLfloat * m_vertices;
  GLfloat * m_normals;
  int m_numVertices;
  unsigned int * m_indexes;
  int m_numTriangles;

  //PARAMETERS
  float m_neighborDistance;
  float m_cs; // Coeficiente de espalhamento
  float m_ca; // Coeficiente de absorção
  float m_g; // cosseno médio do ângulo de espalhamento [-1,1]. pele humana = (0,7 até 0.9) Alto Subsurface Scatter
  float m_n1, m_n2; // indices de refração dos materiais

  //OUTPUT
  VertexInfo * m_vertexInfo;
  vector<float> * m_vertexNeighbor;
  vector<float> * m_vertexR;

public:
  PreProcessor(GLfloat * vertices, GLfloat * normals, int numVertices, unsigned int * indexes, int numTriangles);
  ~PreProcessor();

  float getNeighborDistance() const;
  void setNeighborDistance(float val);

  float getCs() const;
  void setCs(float val);

  float getCa() const;
  void setCa(float val);

  float getG() const;
  void setG(float val);

  float getN2() const;
  float getN1() const;
  void setN(float n1, float n2);

  void calc();

  void writeTextures(string fileName);

protected:
  void calcNeighborhood();
  void calcLightTerms(); // Calc`s R, Q, q
  void calcArea();

  
};

