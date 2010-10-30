/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cmath>

#include "main.h"
#include "PreProcess/PreProcessor.h"

using namespace std;

Vector3 g_lightDirection = Vector3(1,1,1);

///////////////////
//~ PreProcessor
//////////////////
PreProcessor::PreProcessor( GLfloat * vertices, GLfloat * normals, int numVertices, unsigned int * indexes, int numTriangles)
:m_vertices(vertices)
,m_normals(normals)
,m_indexes(indexes)
,m_numVertices(numVertices)
,m_numTriangles(numTriangles)
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
  m_vertexInfo = new VertexInfo[m_numVertices];
  m_vertexNeighbor = new vector<float>[m_numVertices];

  long long mean = 0;

  for(int i=0;i<m_numVertices;++i)
  {
    //m_vertexInfo[i].index = i;
    m_vertexInfo[i].indexF = (float)i;
    m_vertexInfo[i].vertex = Vector3(m_vertices[i*3], m_vertices[i*3+1], m_vertices[i*3+2]);
    m_vertexInfo[i].normal = Vector3(m_normals[i*3], m_normals[i*3+1], m_normals[i*3+2]);
    //m_vertexInfo[i].neighborFileIndex = i==0? 0 : m_vertexInfo[i-1].neighborFileIndex + m_vertexInfo[i-1].numNeighbors;
    m_vertexInfo[i].neighborFileIndexF = i==0? 0.0f : 
                    m_vertexInfo[i-1].neighborFileIndexF + m_vertexInfo[i-1].numNeighborsF;

    for(int j=0;j<m_numVertices;++j)
    {
      if(i==j) continue;
      Vector3 vertex = Vector3(m_vertices[j*3], m_vertices[j*3+1], m_vertices[j*3+2]);
      float d =  ~(m_vertexInfo[i].vertex - vertex);
      if(d < m_neighborDistance)
      {
        m_vertexNeighbor[i].push_back((float)j);
      }
    }
    //m_vertexInfo[i].numNeighbors = m_vertexNeighbor[i].size();
    m_vertexInfo[i].numNeighborsF = (float) m_vertexNeighbor[i].size();
    mean += m_vertexNeighbor[i].size();
    if(i%(m_numVertices/10) == 0)
    {
      printf("\b\b\b%02d%%",(int)((((float)i)/m_numVertices)*100 + 1));
      //cout <<  " !!!" << (((float)m_vertexNeighbor[i].size())/m_numVertices)*100 <<endl;
    }
  }
  cout << endl;
  cout << "Media de neighbors por vertice:" << (float)mean/m_numVertices <<endl;
  cout << "Media de neighbors por vertice(%):" << ((((float)mean/m_numVertices)/m_numVertices)*100) <<endl;
}

void PreProcessor::calcArea()
{
  for(unsigned int i=0;i<m_numVertices;++i)
  {
    float area = 0;
    for(int j=0;j<m_numTriangles;++j)
    {
      if(m_indexes[j*3] == i || m_indexes[j*3+1] == i || m_indexes[j*3+2] == i ) 
      {
        Vector3 cross =  (m_vertexInfo[m_indexes[j*3+2]].vertex - m_vertexInfo[m_indexes[j*3]].vertex) ^  (m_vertexInfo[m_indexes[j*3+1]].vertex - m_vertexInfo[m_indexes[j*3]].vertex);
        area+= abs((cross.x + cross.y + cross.z)/2);
      }
    }
    m_vertexInfo[i].area = area/3;
  }
}

void PreProcessor::calcLightTerms()
{
  float cs = m_cs; // Coeficiente de espalhamento
  float ca = m_ca; // Coeficiente de absorção
  float g = m_g; // cosseno médio do ângulo de espalhamento [-1,1]. pele humana = (0,7 até 0.9) Alto Subsurface Scatter
  float n1 = m_n1, n2 = m_n2; // indices de refração dos materiais
  
  float cs_l = (1 - g)*cs; // Coeficiente reduzido de espalhamento --- cs`
  
  float ct = ca + cs; // Coeficiente de extinsão
  float ct_l = cs_l + ca; // Coeficiente reduzido de extinsão --- ct`
  float ctr = sqrt(3*ca*ct_l); // Coeficiente efetivo de extinsão
  
  float a = cs_l/ct_l; // coeficiente de albedo reduzido --- alfa

  float lu = 1.0/ct_l; // distância média do caminho livre

  float n = n1/n2; // indice relativo de refração
  float Fdr = -1.44/(n*n) + .71/n + .668 + .0636*n; // Termo difuso de Fresnel aproximado por n

  float A  = (1 + Fdr)/(1 - Fdr);
  
  float zr = lu;
  float zv = lu*(1 + (4 * A)/3);


  //Calc Optimizers 
  float zrzr = zr*zr;
  float zvzv = zv*zv;

  float a_4pi = a/(4*PI);
  float e_pwr_ctr = pow(EULER,-ctr);

  Vector3 wi = g_lightDirection.unitary();

  m_vertexR = new vector<float>[m_numVertices];
  for(unsigned int i=0;i<m_numVertices;++i)
  {
    m_vertexInfo[i].Q = Vector3(0,0,0);
    m_vertexInfo[i].q = 0;
    vector<float>::iterator neighborIt =  m_vertexNeighbor[i].begin();
    for(; neighborIt!=m_vertexNeighbor[i].end();++neighborIt)
    {
      // Dipole Aproximation
      // Dipole Aproximation
      float r =  ~(m_vertexInfo[(int)*neighborIt].vertex - m_vertexInfo[i].vertex); 
      float rr = r*r;
      
      float dr = sqrt(rr+zrzr);
      float dv = sqrt(rr+zvzv);

      float Rd = a_4pi*(  zr*(ctr + 1/dr)*pow(e_pwr_ctr, dr)/(dr*dr) + 
                          zv*(ctr + 1/dv)*pow(e_pwr_ctr, dv)/(dv*dv)  );

      m_vertexR[i].push_back(Rd);


      float cosOi = wi * m_vertexInfo[(int)*neighborIt].normal;
      if(cosOi < 0) // NO Contribuition 
      {
        continue;
        //Q
        //Q
        m_vertexInfo[i].Q += Vector3(0,0,0);

        //q
        //q
        m_vertexInfo[i].q += 0;
      }else 
      {
        float sinOi = sin(acos(cosOi));

        float sqrt_1_nsinOi2 = sqrt(1. - (n * sinOi)*(n * sinOi));
        
        float rs =  pow(
                      (n1 * cosOi - n2 * sqrt_1_nsinOi2) /
                      (n1 * cosOi + n2 * sqrt_1_nsinOi2)
                    , 2);
        
        float rp =  pow(
                      (n1 * sqrt_1_nsinOi2 - n2 * cosOi) /
                      (n1 * sqrt_1_nsinOi2 + n2 * cosOi)
                    , 2);
        // Fresnel Term
        // Fresnel Term
        float fr = (rs + rp) / 2.;

        float ft = 1. - fr;

        //Q
        //Q
        m_vertexInfo[i].Q += m_vertexInfo[(int)*neighborIt].normal *
                              ft * (Rd / PI) * m_vertexInfo[(int)*neighborIt].area;

        //q
        //q
        m_vertexInfo[i].q += ft * (Rd / PI) * m_vertexInfo[(int)*neighborIt].area
                                 * cosOi; // ni dot wi
      }
    }
      
    //m_vertexInfo[i].RFileIndex = i==0? 0 : m_vertexInfo[i-1].RFileIndex + m_vertexInfo[i-1].numNeighbors;
    m_vertexInfo[i].RFileIndexF = i==0? 0.0f : 
      m_vertexInfo[i-1].RFileIndexF + m_vertexInfo[i-1].numNeighborsF;
  }
}

float PreProcessor::getCs() const
{
  return m_cs;
}

void PreProcessor::setCs( float val )
{
  m_cs = val;
}

float PreProcessor::getCa() const
{
  return m_ca;
}

void PreProcessor::setCa( float val )
{
  m_ca = val;
}

float PreProcessor::getG() const
{
  return m_g;
}

void PreProcessor::setG( float val )
{
  m_g = val;
}

float PreProcessor::getN2() const
{
  return m_n2;
}

float PreProcessor::getN1() const
{
  return m_n1;
}

void PreProcessor::setN( float n1, float n2 )
{
  m_n1 = n1;  m_n2 = n2;
}

void PreProcessor::writeTextures( string fileName )
{
  int sizeofVertexInfo = sizeof(VertexInfo);
  int vertexInfoSize = m_numVertices*sizeof(VertexInfo);
  int vertexNeighborSize = 0;
  int vertexRSize = 0;
  for(int i = 0; i<m_numVertices; ++i)
  {
    vertexNeighborSize += ((int)m_vertexInfo[i].numNeighborsF)*sizeof(int);
    
    vertexRSize += ((int)m_vertexInfo[i].numNeighborsF)*sizeof(float);
  }

  FILE *fp;
  fp = fopen(fileName.c_str(), "wb");
  MyAssert("Invalid FileName: " + fileName, fp);
  
  fwrite(&m_numVertices, sizeof(int), 1, fp);
  fwrite(&sizeofVertexInfo, sizeof(int), 1, fp);
  fwrite(&vertexInfoSize, sizeof(int), 1, fp);
  fwrite(&vertexNeighborSize, sizeof(int), 1, fp);
  fwrite(&vertexRSize, sizeof(int), 1, fp);
  
  fwrite(m_vertexInfo, sizeof(VertexInfo), m_numVertices, fp );
  
  float *neighbors = new float[vertexNeighborSize/sizeof(float)];
  int *Rs = new int[vertexRSize/sizeof(float)];
  int j = 0;
  int k = 0;
  for(int i = 0; i<m_numVertices; ++i)
  {
    vector<float>::iterator it(m_vertexNeighbor[i].begin());
    for(; it != m_vertexNeighbor[i].end();++it)
    {
      neighbors[j] = *it;
      j++;
    }

    vector<float>::iterator itR(m_vertexR[i].begin());
    for(; itR != m_vertexR[i].end();++itR)
    {
      Rs[k] = *itR;
      k++;
    }
  }
  fwrite(neighbors, sizeof(float), vertexNeighborSize/sizeof(float), fp );
  fwrite(Rs, sizeof(float), vertexRSize/sizeof(float), fp );
  fclose(fp);
}

void PreProcessor::calc()
{
  cout << "Start Calculating Neighborhood..."<<endl;
  calcNeighborhood();
  cout << "Done."<<endl;
  
  cout << "Start Calculating Triangles Area..."<<endl;
  calcArea();
  cout << "Done."<<endl;

 
  cout << "Start Calculating LighTerms..."<<endl;
  calcLightTerms(); // Calc`s R, Q, q
  cout << "Done."<<endl;
}