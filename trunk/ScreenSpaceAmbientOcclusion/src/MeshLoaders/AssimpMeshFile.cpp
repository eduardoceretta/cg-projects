/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
// Baseado em SGPLYLoader de fabraham@tecgraf.puc-rio.br  em Oct 2004

#include "MeshLoaders/AssimpMeshFile.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include "main.h"
#include "assimp/assimp.h"
#include "assimp/aiPostProcess.h"
#include "assimp/aiScene.h"
#include "assimp/aiConfig.h"

AssimpMeshFile::AssimpMeshFile(void):MeshFileBase()
,m_bb_min(Vector3(999999999.,999999999.,999999999.))
,m_bb_max(Vector3(-999999999.,-999999999.,-999999999.))
{
  m_fileType = "NOFILE";
}

AssimpMeshFile::~AssimpMeshFile(void)
{
}

void AssimpMeshFile::readFile( string fileName, Vector3 pos /*= Vector3(0,0,0)*/, Vector3 scale /*= Vector3(1,1,1)*/ )
{
  m_vbo = NULL;
  m_pos = pos;
  m_scale = scale;
  m_fileName = fileName;
  calcTriangles();
}


void AssimpMeshFile::calcVBO()
{
  m_vbo = new VertexBufferObject();
  m_vbo->setVBOBuffer( GL_VERTEX_ARRAY, GL_FLOAT, m_numVertices, m_vertices);
  m_vbo->setVBOBuffer( GL_NORMAL_ARRAY, GL_FLOAT, m_numVertices, m_normals);
  m_vbo->setVBOIndexBuffer(GL_UNSIGNED_INT, m_numTriangles*3, m_indexes);
  m_vbo->calcVBO();
  writeBinaryFile(m_fileName);
}

void AssimpMeshFile::writeBinaryFile(string fileName)
{
  int index = fileName.find_last_of(".");
  MyAssert("Invalid FileName: " + fileName, index!=string::npos);
  string sub = fileName.substr(0, index);

  FILE * fp = fopen((sub+".msb").c_str(),"wb");
  m_vbo->writeToFile(fp);
  fclose(fp);
  cout << "File " << sub+".msb" << " write successfully! " <<endl;
}




void AssimpMeshFile::calcTriangles()
{
  aiSetImportPropertyInteger(AI_CONFIG_PP_RVC_FLAGS
            , aiComponent_TANGENTS_AND_BITANGENTS 	 
            | aiComponent_COLORS
            | aiComponent_TEXCOORDS
            | aiComponent_BONEWEIGHTS 
            | aiComponent_ANIMATIONS
            | aiComponent_TEXTURES 
            | aiComponent_LIGHTS 
            | aiComponent_CAMERAS 
            | aiComponent_MATERIALS
            );

  m_aiscene = aiImportFile(m_fileName.c_str()
            , aiProcess_Triangulate
            | aiProcess_GenSmoothNormals
            | aiProcess_RemoveComponent 
            );
  if (!m_aiscene)
  {
    MyAssert("Could NOT OPEN FILE!" + m_fileName, false);
  }


  struct aiNode* nd  = m_aiscene->mRootNode;

  int nv = 0;
  int nt = 0;
  getModelSizes(nd, nv, nt);

  GLfloat * vList = new GLfloat[nv*3];
  GLfloat * nList = new GLfloat[nv*3]; 
  unsigned int * iList = new unsigned int[nt*3];
  
  int vertexOffset = 0, indexOffset = 0;

  printf("Reading %d Vertices...\n", nv);
  printf("Reading %d Triagnles...\n", nt);

  struct aiMatrix4x4 transform;
  aiIdentityMatrix4(&transform);

  createVbo(nd, vList, nList, iList, vertexOffset, indexOffset, &transform);

  
  cout << "TRANSLATE TO:"<<((m_bb_max - m_bb_min)*.5) <<endl;

  m_numVertices = nv;
  m_numTriangles = nt;
  m_vertices = vList;
  m_normals = nList;
  m_indexes = iList;

  aiReleaseImport(m_aiscene);
}


void AssimpMeshFile::getModelSizes(struct aiNode* nd, int &nVertices, int &nTriangles)
{
  int childrenVertices = 0;
  int childrenTriangles = 0;
  for(int i = 0; i < nd->mNumChildren; ++i)
  {
    int v = 0;
    int t = 0;
    getModelSizes(nd->mChildren[i], v, t);
    childrenVertices += v;
    childrenTriangles += t;
  }
  int myVertices = 0;
  int myTriangles = 0;
  for(int i = 0; i < nd->mNumMeshes; ++i)
  {
    const struct aiMesh* mesh = m_aiscene->mMeshes[nd->mMeshes[i]];
    myVertices += mesh->mNumVertices;
    myTriangles += mesh->mNumFaces;
  }
  nVertices += myVertices + childrenVertices;
  nTriangles += myTriangles + childrenTriangles;
}


void  AssimpMeshFile:: createVbo(struct aiNode* nd, GLfloat * vList, GLfloat * nList, 
                                 unsigned int * iList, int &vertexOffset, int &indexOffset, struct aiMatrix4x4* trafo)
{
  struct aiMatrix4x4 prev;
  prev = *trafo;
  aiMultiplyMatrix4(trafo, &nd->mTransformation);

  for (int n = 0; n < nd->mNumMeshes; ++n) {
    const struct aiMesh* mesh = m_aiscene->mMeshes[nd->mMeshes[n]];

    for(int i = 0; i<mesh->mNumVertices; ++i) {
      struct aiVector3D vertex = mesh->mVertices[i];
      aiTransformVecByMatrix4(&vertex,trafo);

      vList[vertexOffset + i*3] = vertex.x * m_scale.x;
      vList[vertexOffset + i*3+1] = vertex.y* m_scale.y;
      vList[vertexOffset + i*3+2] = vertex.z* m_scale.z;

      nList[vertexOffset + i*3] = mesh->mNormals[i].x;
      nList[vertexOffset + i*3+1] = mesh->mNormals[i].y;
      nList[vertexOffset + i*3+2] = mesh->mNormals[i].z;

      m_bb_min.x = min(vertex.x * m_scale.x, m_bb_min.x);
      m_bb_min.y = min(vertex.y * m_scale.y, m_bb_min.y);
      m_bb_min.z = min(vertex.z * m_scale.z, m_bb_min.z);

      m_bb_max.x = max(vertex.x * m_scale.x, m_bb_max.x);
      m_bb_max.y = max(vertex.y * m_scale.y, m_bb_max.y);
      m_bb_max.z = max(vertex.z * m_scale.z, m_bb_max.z);

    }
    

    for (int t = 0; t < mesh->mNumFaces; ++t) {
      const struct aiFace* face = &mesh->mFaces[t];
      GLenum face_mode;

      if(face->mNumIndices != 3)
      {
        MyAssert("ONLY TRIANGLES ACCPTED!", false);
      }

      for(int i = 0; i < face->mNumIndices; ++i) {
        int index = face->mIndices[i];
        iList[indexOffset + t*3+i] = vertexOffset/3 + index;
      }
    }
    vertexOffset += mesh->mNumVertices*3;
    indexOffset += mesh->mNumFaces*3;
  }
  
  for(int i = 0; i < nd->mNumChildren; ++i)
  {
    createVbo(nd->mChildren[i], vList, nList, iList, vertexOffset, indexOffset, trafo);
  }
  *trafo = prev;
}


bool AssimpMeshFile::isValidFileType(string filetype)
{
  return aiIsExtensionSupported(filetype.c_str());
}




