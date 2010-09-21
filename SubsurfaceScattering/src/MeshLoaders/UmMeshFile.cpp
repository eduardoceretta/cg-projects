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

VertexBufferObject* UmMeshFile::readFileToVBO( unsigned int materialIndex, string fileName, Vector3 pos, Vector3 scale )
{
  readFileTriangles(fileName, materialIndex, pos, scale);
  calcVBO();
  writeBinaryFile (fileName);

  return m_vbo;
}


void UmMeshFile::calcVBO()
{
  m_vbo = new VertexBufferObject();

  m_vbo->setVBOBuffer( GL_VERTEX_ARRAY, GL_FLOAT, m_numVertices, m_vertices);
  m_vbo->setVBOBuffer( GL_NORMAL_ARRAY, GL_FLOAT, m_numVertices, m_normals);
  m_vbo->calcVBO();
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

void UmMeshFile::readFileTriangles( string fileName, unsigned int materialIndex /*= 0*/, Vector3 pos /*= Vector3(0,0,0)*/, Vector3 scale /*= Vector3(1,1,1)*/ )
{
  m_pos = pos;
  m_scale = scale;
  m_materialIndex = materialIndex;
  calcTriangles(fileName);
  calcTrianglesArrays();
}

void UmMeshFile::calcTriangles( string fileName )
{
  FILE *file;
  file = fopen(fileName.c_str(), "rt");
  MyAssert("File Not Found: " + fileName, file);

  int numVertex, num_Triangles;

  fscanf(file, "%d\n", &numVertex);
  printf("Reading %d Vertices...\n", numVertex);

  Vector3 * vList = new Vector3[numVertex];
  Vector3 * nList = new Vector3[numVertex];

  for(int i = 0; i < numVertex; ++i)
  {
    nList[i] = Vector3(0,0,0);
    fscanf(file, "%f %f %f\n", &vList[i].x, &vList[i].y, &vList[i].z); 

    vList[i].x*=m_scale.x;
    vList[i].y*=m_scale.y;
    vList[i].z*=m_scale.z;

    //vList[i].x+=pos.x;
    //vList[i].y+=pos.y;
    //vList[i].z+=pos.z;
  }

  fscanf(file, "\n%d\n", &num_Triangles);
  printf("Reading %d Triangles...\n", num_Triangles);

  Vector3 * tList = new Vector3[num_Triangles];

  for(int i = 0; i < num_Triangles; ++i)
  {
    int t1, t2, t3;
    fscanf(file, "%d %d %d\n", &t1, &t2, &t3);

    nList[t1] += (vList[t2] - vList[t1]) ^ (vList[t3] - vList[t2]);
    nList[t2] += (vList[t2] - vList[t1]) ^ (vList[t3] - vList[t2]);
    nList[t3] += (vList[t2] - vList[t1]) ^ (vList[t3] - vList[t2]);

    tList[i] = Vector3(t1,t2,t3);
  }

  for(int i = 0; i < numVertex; ++i)
  {
    nList[i] = nList[i].unitary();
  }

  m_triangles.reserve(num_Triangles);
  for(int i = 0; i < num_Triangles; ++i)
  {
    Vector3 t = tList[i];
    int t1 = t.x, t2 = t.y, t3 = t.z;

    Triangle rt(m_materialIndex, vList[t1], vList[t2], vList[t3], nList[t1], nList[t2], nList[t3] );
    m_triangles.push_back(rt);
  }

  fclose(file);

  delete[] vList;
  delete[] nList;
  delete[] tList;
}

void UmMeshFile::calcTrianglesArrays()
{
  m_numVertices = m_triangles.size()*3;
  m_vertices = new GLfloat[m_numVertices*3];
  m_normals = new GLfloat[m_numVertices*3];

  for(int i = 0; i < m_numVertices/3; ++i)
  {
    int index = i*9;
    m_normals[index] = m_triangles[i].n1.x;
    m_vertices[index++] = m_triangles[i].v1.x;
    m_normals[index] = m_triangles[i].n1.y;
    m_vertices[index++] = m_triangles[i].v1.y;
    m_normals[index] = m_triangles[i].n1.z;
    m_vertices[index++] = m_triangles[i].v1.z;
    m_normals[index] = m_triangles[i].n2.x;
    m_vertices[index++] = m_triangles[i].v2.x;
    m_normals[index] = m_triangles[i].n2.y;
    m_vertices[index++] = m_triangles[i].v2.y;
    m_normals[index] = m_triangles[i].n2.z;
    m_vertices[index++] = m_triangles[i].v2.z;
    m_normals[index] = m_triangles[i].n3.x;
    m_vertices[index++] = m_triangles[i].v3.x;
    m_normals[index] = m_triangles[i].n3.y;
    m_vertices[index++] = m_triangles[i].v3.y;
    m_normals[index] = m_triangles[i].n3.z;
    m_vertices[index++] = m_triangles[i].v3.z;
  }
}