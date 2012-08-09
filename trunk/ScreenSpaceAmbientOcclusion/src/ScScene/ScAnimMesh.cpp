/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Mar 2010
 *
 *  OpenGL Mesh Attributes Container
 *  A VBO/P3BMesh with a material and a transformation
 */

#include <iostream>
#include <limits>
#include "defines.h"
#include "ScScene/ScAnimMesh.h"

#include "GLUtils/GLAnimationObject.h"
#include "MeshLoaders/KeyFrameMesh.h"

using namespace std;

int ScAnimMesh :: s_animMeshNum = 0;


ScAnimMesh :: ScAnimMesh()
:m_animObj(NULL)
,m_animOutdated(true)
,m_animSpeed(0.0f)
,m_animType("Regular")
{
  m_myAnimMeshNum = s_animMeshNum++;
}

ScAnimMesh :: ~ScAnimMesh()
{
  vector<KeyFrameMesh*> :: reverse_iterator kIt;
  for(kIt = m_keyFrameMeshes.rbegin(); kIt != m_keyFrameMeshes.rend(); ++kIt)
    delete *kIt;

  if(m_animObj)
    delete m_animObj;
}


int ScAnimMesh :: getMyAnimMeshNumber() const
{
   return m_myAnimMeshNum;
}

void ScAnimMesh :: readFromStr(char buffer[])
{
  //////////////////////////////////////////////////////////////////////////
  // Read data
  char animType[100];
  int r = sscanf( buffer, "%d %f %f %f %f %f %f %f %s %[^\n]s", &m_materialIndex, &m_pos.x, &m_pos.y, &m_pos.z,
    &m_scale.x, &m_scale.y, &m_scale.z, &m_animSpeed, animType, buffer);
  MyAssert("----\nNot Enough Parameters in MeshAnim!\nMust have material pos(3) scale(3) keyframes(n).\n----", r == 10);
  m_animType = string(animType);

  char *name = new char[strlen(buffer) + 1]; 
  while(sscanf( buffer, "%s %[^\n]s", name, buffer) == 2)
  {
    string fileName(name);
    m_keyFrameFileNames.push_back(fileName);
  }

  ////Last FileName
  string fileName(name);
  m_keyFrameFileNames.push_back(fileName);

  delete name;
  MyAssert("----\nNot Enough Key Frames in MeshAnim!\nMust have at least two key frames.\n----", m_keyFrameFileNames.size() > 1);

  m_animOutdated = true;
}


int ScAnimMesh :: getMaterialIndex()
{
   return m_materialIndex;
}


void ScAnimMesh :: configure()
{
  if(m_animOutdated == true)
  {
    //////////////////////////////////////////////////////////////////////////
    // Create KeyFrames
    vector<string> :: iterator it;
    for(it = m_keyFrameFileNames.begin(); it != m_keyFrameFileNames.end(); ++it)
    {
      string fileName = *it;

      int index = fileName.find_last_of(".");
      int indexSlash = fileName.find_last_of("/");
      MyAssert("Invalid FileName: " + fileName, index != string::npos);
      string sub = fileName.substr(index, string::npos);

      //*it = fileName.substr(indexSlash + 1, index - indexSlash - 1);

      KeyFrameMesh *kf = new KeyFrameMesh();
      if(kf->isValidFileType(sub))
      {
        kf->readFile(fileName, m_pos, m_scale);
        m_keyFrameMeshes.push_back(kf);
      }else 
        delete kf;
    }

    //////////////////////////////////////////////////////////////////////////
    // Setup Animation
    if(m_animObj == NULL)
    {
      GLAnimationObject::AnimType t = GLAnimationObject::Regular;
      if(m_animType == "Reverse")
        t = GLAnimationObject::Reverse; 
      else if(m_animType == "Loop")
        t = GLAnimationObject::Loop; 

      m_animObj = new GLAnimationObject(m_animSpeed, t);
    }

    vector<KeyFrameMesh*> :: iterator kIt;
    for(kIt = m_keyFrameMeshes.begin(); kIt != m_keyFrameMeshes.end(); ++kIt)
      m_animObj->addKeyFrame(*kIt);
    m_animOutdated = false;
  }  

  if(m_animObj)
    m_animObj->configure();
}

void ScAnimMesh :: render()
{
  if(m_animObj)
  {
    glPushMatrix();
      glTranslatef(m_pos.x, m_pos.y, m_pos.z);
      m_animObj->render(false);
    glPopMatrix();
  }
}

GLAnimationObject* ScAnimMesh::getAnimationObject() const
{
  return m_animObj;
}

Vector3 ScAnimMesh::getBoundingBoxSize() const
{
  Vector3 bb_max = getBoundingBoxMax();
  Vector3 bb_min = getBoundingBoxMin();
  return m_animObj->getBoundingBoxSize(0);
}

Vector3 ScAnimMesh::getBoundingBoxCenter() const
{
  Vector3 bb_max = getBoundingBoxMax();
  Vector3 bb_min = getBoundingBoxMin();
  return (bb_max + bb_min)*.5;
}

Vector3 ScAnimMesh::getBoundingBoxMin() const
{
  if(m_animObj)
    return  m_pos + m_animObj->getBoundingBoxMin();

  return Vector3(-numeric_limits<float>::infinity( ), -numeric_limits<float>::infinity( ), -numeric_limits<float>::infinity( ));
}

Vector3 ScAnimMesh::getBoundingBoxMax() const
{
  if(m_animObj)
    return  m_pos + m_animObj->getBoundingBoxMax();

  return Vector3(numeric_limits<float>::infinity( ), numeric_limits<float>::infinity( ), numeric_limits<float>::infinity( ));
}

string ScAnimMesh::getFileName(int i) const
{
  return m_keyFrameFileNames.at(i);
}



