/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#include "MeshLoaders/UmMeshFile.h"
#include <string>
#include "main.h"

UmMeshFile::UmMeshFile(void):MeshFileBase()
{
  m_fileType = ".um";
}

UmMeshFile::~UmMeshFile(void)
{
}

void UmMeshFile::readFile( string fileName, Vector3 pos /*= Vector3(0,0,0)*/, Vector3 scale /*= Vector3(1,1,1)*/ )
{
  m_pos = pos;
  m_scale = scale;
  m_fileName = fileName;
  calcTriangles();
}

void UmMeshFile::calcVBO()
{
  m_vbo = new VertexBufferObject();
  m_vbo->setVBOBuffer( GL_VERTEX_ARRAY, GL_FLOAT, m_numVertices, m_vertices);
  m_vbo->setVBOBuffer( GL_NORMAL_ARRAY, GL_FLOAT, m_numVertices, m_normals);
  m_vbo->setVBOIndexBuffer(GL_UNSIGNED_INT, m_numTriangles*3, m_indexes);
  m_vbo->calcVBO();
  writeBinaryFile(m_fileName);
}

void UmMeshFile::writeBinaryFile(string fileName)
{
  int index = fileName.find_last_of(".");
  MyAssert("Invalid FileName: " + fileName, index!=string::npos);
  string sub = fileName.substr(0, index);

  FILE * fp = fopen((sub+".msb").c_str(),"wb");
  m_vbo->writeToFile(fp);
  fclose(fp);
  cout << "File " << sub+".msb" << " write successfully! " <<endl;
}


void UmMeshFile::calcTriangles()
{
  FILE *file;
  file = fopen(m_fileName.c_str(), "rt");
  MyAssert("File Not Found: " + m_fileName, file);

  int numVertex, numTriangles;

  fscanf(file, "%d\n", &numVertex);
  printf("Reading %d Vertices...\n", numVertex);

  GLfloat * vList = new GLfloat[numVertex];
  GLfloat * nList = new GLfloat[numVertex];

  for(int i = 0; i < numVertex; ++i)
  {
    fscanf(file, "%f %f %f\n", &vList[i*3], &vList[i*3+1], &vList[i*3+2]); 

    vList[i*3]*=m_scale.x;
    vList[i*3+1]*=m_scale.y;
    vList[i*3+2]*=m_scale.z;

    //vList[i].x+=pos.x;
    //vList[i].y+=pos.y;
    //vList[i].z+=pos.z;

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

    nList[t1*3] += normal.x;
    nList[t1*3+1] += normal.y;
    nList[t1*3+2] += normal.z;
    nList[t2*3] += normal.x;
    nList[t2*3+1] += normal.y;
    nList[t2*3+2] += normal.z;
    nList[t3*3] += normal.x;
    nList[t3*3+1] += normal.y;
    nList[t3*3+2] += normal.z;

    iList[i*3] = t1;
    iList[i*3+1] = t2;
    iList[i*3+2] = t3;
  }

  for(int i = 0; i < numVertex; ++i)
  {
    Vector3 normal(nList[i*3], nList[i*3+1], nList[i*3+2]);
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
