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

VertexBufferObject* UmMeshFile::readFile( unsigned int materialIndex, string fileName, Vector3 pos, Vector3 scale )
{
  m_pos = pos;
  m_scale = scale;

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

    vList[i].x*=scale.x;
    vList[i].y*=scale.y;
    vList[i].z*=scale.z;

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

    Triangle rt(materialIndex, vList[t1], vList[t2], vList[t3], nList[t1], nList[t2], nList[t3] );
    m_triangles.push_back(rt);
  }

  fclose(file);

  delete[] vList;
  delete[] nList;
  delete[] tList;

  calcVBO();
  //applyScaleTranslateToVertexes();
  writeBinaryFile (fileName);

  return m_vbo;
}


void UmMeshFile::calcVBO()
{
  m_vbo = new VertexBufferObject();

  int n = m_triangles.size()*3;
  GLfloat* vertices = new GLfloat[n*3];
  GLfloat* normals = new GLfloat[n*3];

  for(int i = 0; i < n/3; ++i)
  {
    int index = i*9;
    normals[index] = m_triangles[i].n1.x;
    vertices[index++] = m_triangles[i].v1.x;
    normals[index] = m_triangles[i].n1.y;
    vertices[index++] = m_triangles[i].v1.y;
    normals[index] = m_triangles[i].n1.z;
    vertices[index++] = m_triangles[i].v1.z;
    normals[index] = m_triangles[i].n2.x;
    vertices[index++] = m_triangles[i].v2.x;
    normals[index] = m_triangles[i].n2.y;
    vertices[index++] = m_triangles[i].v2.y;
    normals[index] = m_triangles[i].n2.z;
    vertices[index++] = m_triangles[i].v2.z;
    normals[index] = m_triangles[i].n3.x;
    vertices[index++] = m_triangles[i].v3.x;
    normals[index] = m_triangles[i].n3.y;
    vertices[index++] = m_triangles[i].v3.y;
    normals[index] = m_triangles[i].n3.z;
    vertices[index++] = m_triangles[i].v3.z;
  }

  m_vbo->setVBOBuffer( GL_VERTEX_ARRAY, GL_FLOAT, n, vertices);
  m_vbo->setVBOBuffer( GL_NORMAL_ARRAY, GL_FLOAT, n, normals);
  m_vbo->calcVBO();
}

void UmMeshFile::applyScaleTranslateToVertexes() 
{
  vector<Triangle> :: iterator tIt;
  for(tIt = m_triangles.begin(); tIt != m_triangles.end(); ++tIt){
    //tIt->v1.x *= m_scale.x;
    //tIt->v1.y *= m_scale.y;
    //tIt->v1.z *= m_scale.z;

    //tIt->v2.x *= m_scale.x;
    //tIt->v2.y *= m_scale.y;
    //tIt->v2.z *= m_scale.z;

    //tIt->v3.x *= m_scale.x;
    //tIt->v3.y *= m_scale.y;
    //tIt->v3.z *= m_scale.z;

    tIt->v1.x += m_pos.x;
    tIt->v1.y += m_pos.y;
    tIt->v1.z += m_pos.z;

    tIt->v2.x += m_pos.x;
    tIt->v2.y += m_pos.y;
    tIt->v2.z += m_pos.z;

    tIt->v3.x += m_pos.x;
    tIt->v3.y += m_pos.y;
    tIt->v3.z += m_pos.z;
  }
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

