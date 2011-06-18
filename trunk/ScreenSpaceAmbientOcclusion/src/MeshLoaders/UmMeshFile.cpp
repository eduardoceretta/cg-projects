/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Mar 2011
 *
 *  UM Model Loader. 
 *  Imports a text mesh file that contains a list of vertexes 
 *  and a list of triangles.
 */
#include "MeshLoaders/UmMeshFile.h"
#include <string>
#include "defines.h"

UmMeshFile::UmMeshFile(void):MeshFileBase()
{
  m_fileType = ".um";
}

UmMeshFile::~UmMeshFile(void)
{
}

void UmMeshFile::readFile( string fileName, Vector3 pos /*= Vector3(0,0,0)*/, Vector3 scale /*= Vector3(1,1,1)*/ )
{
  m_vbo = NULL;
  m_pos = pos;
  m_scale = scale;
  m_fileName = fileName;
  calcTriangles();
}

void UmMeshFile::calcVBO()
{
  m_vbo = new GLVertexBufferObject();
  m_vbo->setVBOBuffer( GL_VERTEX_ARRAY, GL_FLOAT, m_numVertices, m_vertices);
  m_vbo->setVBOBuffer( GL_NORMAL_ARRAY, GL_FLOAT, m_numVertices, m_normals);
  m_vbo->setVBOIndexBuffer(GL_UNSIGNED_INT, m_numTriangles*3, m_indexes);
  m_vbo->calcVBO();
  if(m_writeBinaryFile)
    writeBinaryFile(m_fileName);
}

void UmMeshFile::calcTriangles()
{
  FILE *file;
  file = fopen(m_fileName.c_str(), "rt");
  MyAssert("File Not Found: " + m_fileName, file);

  int numVertex, numTriangles;

  fscanf(file, "%d\n", &numVertex);
  printf("Reading %d Vertices...\n", numVertex);

  GLfloat * vList = new GLfloat[numVertex*3];
  GLfloat * nList = new GLfloat[numVertex*3];

  for(int i = 0; i < numVertex; ++i)
  {
    fscanf(file, "%f %f %f\n", &vList[i*3], &vList[i*3+1], &vList[i*3+2]); 

    vList[i*3]*=m_scale.x;
    vList[i*3+1]*=m_scale.y;
    vList[i*3+2]*=m_scale.z;

    nList[i*3] = 0;
    nList[i*3+1] = 0;
    nList[i*3+2] = 0;
  }

  fscanf(file, "\n%d\n", &numTriangles);
  printf("Reading %d Triangles...\n", numTriangles);
  unsigned int * iList = new unsigned int[numTriangles*3];

  for(int i = 0; i < numTriangles; ++i)
  {
    unsigned int t1, t2, t3;
    fscanf(file, "%u %u %u\n", &t1, &t2, &t3);

    Vector3 v1(vList[t1*3], vList[t1*3+1], vList[t1*3+2]);
    Vector3 v2(vList[t2*3], vList[t2*3+1], vList[t2*3+2]);
    Vector3 v3(vList[t3*3], vList[t3*3+1], vList[t3*3+2]);
    Vector3 normal = (v2 - v1) ^ (v3 - v1);
    normal = normal.unitary();

    Vector3 nListT1(nList[t1*3], nList[t1*3+1], nList[t1*3+1]);
    Vector3 normalT1 = (nListT1 + normal).unitary();
    nList[t1*3] = normalT1.x;
    nList[t1*3+1] = normalT1.y;
    nList[t1*3+2] = normalT1.z;

    Vector3 nListT2(nList[t2*3], nList[t2*3+1], nList[t2*3+1]);
    Vector3 normalT2 = (nListT2 + normal).unitary();
    nList[t2*3] = normalT2.x;
    nList[t2*3+1] = normalT2.y;
    nList[t2*3+2] = normalT2.z;

    Vector3 nListT3(nList[t3*3], nList[t3*3+1], nList[t3*3+1]);
    Vector3 normalT3 = (nListT3 + normal).unitary();
    nList[t3*3] = normalT3.x;
    nList[t3*3+1] = normalT3.y;
    nList[t3*3+2] = normalT3.z;

    iList[i*3] = t1;
    iList[i*3+1] = t2;
    iList[i*3+2] = t3;
  }

  for(int i = 0; i < numVertex; ++i)
  {
    Vector3 normal(nList[i*3], nList[i*3+1], nList[i*3+2]);
    if(normal == Vector3(0,0,0))
      normal = Vector3(0,1,0);
    normal = normal.unitary();

    nList[i*3] = normal.x;
    nList[i*3+1] = normal.y;
    nList[i*3+2] = normal.z;
  }

  fclose(file);

  m_numVertices = numVertex;
  m_numTriangles = numTriangles;
  m_vertices = vList;
  m_normals = nList;
  m_indexes = iList;
}
