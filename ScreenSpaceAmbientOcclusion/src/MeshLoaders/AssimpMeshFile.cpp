/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  May 2011
 *
 *  Model Loader. Imports diverse file types.
 *  Depends of the external Library Open Asset Import Library (Assimp), available on http://assimp.sourceforge.net/
 */

#include "MeshLoaders/AssimpMeshFile.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include "defines.h"
#include "assimp/assimp.h"
#include "assimp/aiPostProcess.h"
#include "assimp/aiScene.h"
#include "assimp/aiConfig.h"

AssimpMeshFile::AssimpMeshFile(void)
:MeshFileBase()
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
  m_vbo = new GLVertexBufferObject();
  m_vbo->setVBOBuffer(GL_VERTEX_ARRAY, GL_FLOAT, m_numVertices, m_vertices);
  m_vbo->setVBOBuffer(GL_NORMAL_ARRAY, GL_FLOAT, m_numVertices, m_normals);
  m_vbo->setVBOIndexBuffer(GL_UNSIGNED_INT, m_numTriangles*3, m_indexes);
  m_vbo->calcVBO();
  if(m_writeBinaryFile)
    writeBinaryFile(m_fileName);
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
            //| aiProcess_GenNormals 
            | aiProcess_GenSmoothNormals
            | aiProcess_FixInfacingNormals
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

  cout << "BoundingBox Size:" << (m_bb_max - m_bb_min) ;
  cout << "BoundingBox Center:"<<((m_bb_max + m_bb_min)*.5) <<endl;

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
                                 unsigned int * iList, int &vertexOffset, int &indexOffset, struct aiMatrix4x4* transform)
{
  struct aiMatrix4x4 prev;
  prev = *transform;
  aiMultiplyMatrix4(transform, &nd->mTransformation);

  for (int n = 0; n < nd->mNumMeshes; ++n) {
    const struct aiMesh* mesh = m_aiscene->mMeshes[nd->mMeshes[n]];

    for(int i = 0; i<mesh->mNumVertices; ++i) {
      struct aiVector3D vertex = mesh->mVertices[i];
      aiTransformVecByMatrix4(&vertex,transform);

      vertex.x *= m_scale.x;
      vertex.y *= m_scale.y;
      vertex.z *= m_scale.z;

      vList[vertexOffset + i*3] =   vertex.x; 
      vList[vertexOffset + i*3+1] = vertex.y;
      vList[vertexOffset + i*3+2] = vertex.z;

      struct aiMatrix4x4 inv = *transform;
      inv.Inverse();
      inv.Transpose();
      struct aiVector3D normal = mesh->mNormals[i];
      aiTransformVecByMatrix4(&normal,&inv);

      nList[vertexOffset + i*3] =  normal.x;
      nList[vertexOffset + i*3+1] = normal.y;
      nList[vertexOffset + i*3+2] = normal.z;

      m_bb_min.x =min(vList[vertexOffset + i*3]  , m_bb_min.x);
      m_bb_min.y = min(vList[vertexOffset + i*3+1], m_bb_min.y);
      m_bb_min.z = min(vList[vertexOffset + i*3+2], m_bb_min.z);

      m_bb_max.x = max(vList[vertexOffset + i*3]  , m_bb_max.x);
      m_bb_max.y = max(vList[vertexOffset + i*3+1], m_bb_max.y);
      m_bb_max.z = max(vList[vertexOffset + i*3+2], m_bb_max.z);
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
    createVbo(nd->mChildren[i], vList, nList, iList, vertexOffset, indexOffset, transform);
  }
  *transform = prev;
}


bool AssimpMeshFile::isValidFileType(string filetype)
{
  return aiIsExtensionSupported(filetype.c_str());
}


