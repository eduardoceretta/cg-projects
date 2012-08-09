/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Mar 2010
 *
 *  Scene file loader and OpenGL scene container
 *  Load Meshes, Materials, Lights and Camera
 */
#include <iostream>
#include <limits>
#include <GL/glew.h>
#include <GL/glut.h>

//#define DEBUG_BB

#include "defines.h"
#include "ScScene/ScScene.h"

using namespace std;

ScScene :: ScScene()
:m_calculated(false)
,m_lightEnabled(true)
,m_screenWidth(0)
,m_screenHeight(0)
,m_numVertices(0)
,m_numElements(0)
{

}

ScScene :: ~ScScene()
{
  vector<ScAnimMesh*> :: reverse_iterator it;
  for(it = m_animMeshes.rbegin(); it != m_animMeshes.rend(); ++it)
    delete *it;
}

void ScScene :: readFromStr(char buffer[])
{
   int r = sscanf( buffer, "%f %f %f %f %f %f %d %d %*s\n", &m_clear.r, &m_clear.g, &m_clear.b,
      &m_ambient.r, &m_ambient.g, &m_ambient.b, &m_screenWidth, &m_screenHeight); 
   assert(r == 9-1);
}

void ScScene :: configure()
{
  if(!m_calculated) 
  {
    vector<ScMesh> :: iterator meshIt;
    for( meshIt = m_meshes.begin(); meshIt!=m_meshes.end(); ++meshIt)
    {
      m_numVertices += meshIt->getNumVertices();
      m_numElements += meshIt->getNumElements();
    }
    m_calculated = true;
  }

  if(m_lightEnabled)
  {
    vector<ScLight> :: iterator lightIt;
    for( lightIt = m_lights.begin(); lightIt!=m_lights.end(); ++lightIt)
    {
      lightIt->configure();
    }
  }

  vector<ScMesh> :: iterator meshIt;
  for( meshIt = m_meshes.begin(); meshIt!=m_meshes.end(); ++meshIt)
  {
    if(m_lightEnabled)
      m_materials[meshIt->getMaterialIndex()].configure();
    meshIt->configure();
  }

  vector<ScAnimMesh*> :: iterator animMeshIt;
  for( animMeshIt = m_animMeshes.begin(); animMeshIt!=m_animMeshes.end(); ++animMeshIt)
  {
    if(m_lightEnabled)
      m_materials[(*animMeshIt)->getMaterialIndex()].configure();
    (*animMeshIt)->configure();
  }
}


void ScScene :: render()
{
  glPushAttrib(GL_LIGHTING_BIT);

    if(m_lightEnabled)
    {
      vector<ScLight> :: iterator lightIt;
      for( lightIt = m_lights.begin(); lightIt!=m_lights.end(); ++lightIt)
        lightIt->render();
    }

    vector<ScMesh> :: iterator meshIt;
    for( meshIt = m_meshes.begin(); meshIt!=m_meshes.end(); ++meshIt)
    {
      glPushAttrib(GL_LIGHTING_BIT);
          m_materials[meshIt->getMaterialIndex()].render();
          meshIt->render();
      glPopAttrib();
    }

    vector<ScAnimMesh*> :: iterator animMeshIt;
    for( animMeshIt = m_animMeshes.begin(); animMeshIt!=m_animMeshes.end(); ++animMeshIt)
    {
      glPushAttrib(GL_LIGHTING_BIT);
          m_materials[(*animMeshIt)->getMaterialIndex()].render();
          (*animMeshIt)->render();
      glPopAttrib();
    }
  glPopAttrib();

#ifdef DEBUG_BB
  Vector3 c = getSceneBoundingBoxCenter();
  Vector3 s = getSceneBoundingBoxSize();
  glPushMatrix();
    glTranslatef(c.x, c.y, c.z);
    glScalef(s.x, s.y, s.z);
    glutWireCube(1);
  glPopMatrix();
#endif // DEBUG_BB
}


bool ScScene::isSceneLightEnabled() const
{
  return m_lightEnabled; 
}

void ScScene::setSceneLightEnabled(bool val) 
{ 
  m_lightEnabled = val; 
}

int ScScene::getNumLights()
{
  return m_lights.size();
}



ScLight* ScScene::getLightAt( int i )
{
  return &m_lights[i];
}

void ScScene::setLightActive( bool op )
{
  if(op && m_lightEnabled)
  {
    if(m_lightEnabled)
    {
      glPushAttrib(GL_LIGHTING_BIT);
     
      vector<ScLight> :: iterator lightIt;
      for( lightIt = m_lights.begin(); lightIt!=m_lights.end(); ++lightIt)
      {
        lightIt->configure();
        lightIt->render();
      }

    }
  }else {
    if(m_lightEnabled)
      glPopAttrib();
  }
}

void ScScene::setMaterialActive( bool op , int index)
{
  if(op)
  {
    glPushAttrib(GL_LIGHTING_BIT);
    m_materials[index].configure();
    m_materials[index].render();
  }else
  {
    glPopAttrib();
  }
}

void ScScene::renderMesh(int index)
{
  m_meshes[index].configure();
  m_meshes[index].render();
}

void ScScene::renderAnimatedMesh(int index)
{
  m_animMeshes[index]->configure();
  m_animMeshes[index]->render();
}

int ScScene::getNumMeshes()
{
  return m_meshes.size();
}

ScMesh* ScScene::getMeshAt( int i )
{
  return &m_meshes[i];
}

int ScScene::getNumAnimatedMeshes()
{
  return m_animMeshes.size();
}

ScAnimMesh* ScScene::getAnimatedMeshAt(int i)
{
  return m_animMeshes.at(i);
}

Color ScScene::getClearColor() const
{
  return m_clear;
}

Color ScScene::getAmbientColor() const
{
  return m_ambient;
}

int ScScene::getScreenWidth() const
{
  return m_screenWidth;
}

int ScScene::getScreenHeight() const
{
  return m_screenHeight;
}

ScCamera* ScScene::getCamera()
{
  return getCameraAt(0);
}

int ScScene::getNumCameras()
{
  return m_cameras.size();
}

ScCamera* ScScene::getCameraAt( int i )
{
  return &m_cameras.at(i);
}

Vector3 ScScene::getSceneBoundingBoxSize() const
{
  Vector3 bb_max = getSceneBoundingBoxMax();
  Vector3 bb_min = getSceneBoundingBoxMin();
  return bb_max - bb_min;
}

Vector3 ScScene::getSceneBoundingBoxCenter() const
{
  Vector3 bb_max = getSceneBoundingBoxMax();
  Vector3 bb_min = getSceneBoundingBoxMin();
  return (bb_max + bb_min)*.5;
}

Vector3 ScScene::getSceneBoundingBoxMin() const
{
  vector<ScMesh> :: const_iterator meshIt;
  Vector3 bb_min = Vector3(numeric_limits<float>::infinity( ), numeric_limits<float>::infinity( ), numeric_limits<float>::infinity( ));
  for( meshIt = m_meshes.begin(); meshIt!=m_meshes.end(); ++meshIt)
  {
    Vector3 mesh = meshIt->getBoundingBoxMin();
    bb_min.x = min(mesh.x, bb_min.x);
    bb_min.y = min(mesh.y, bb_min.y);
    bb_min.z = min(mesh.z, bb_min.z);
  }

  vector<ScAnimMesh*> :: const_iterator animMeshIt;
  for( animMeshIt = m_animMeshes.begin(); animMeshIt!=m_animMeshes.end(); ++animMeshIt)
  {
    Vector3 mesh = (*animMeshIt)->getBoundingBoxMin();
    bb_min.x = min(mesh.x, bb_min.x);
    bb_min.y = min(mesh.y, bb_min.y);
    bb_min.z = min(mesh.z, bb_min.z);
  }

  return bb_min;
}

Vector3 ScScene::getSceneBoundingBoxMax() const
{
  vector<ScMesh> :: const_iterator meshIt;
  Vector3 bb_max = Vector3(-numeric_limits<float>::infinity( ), -numeric_limits<float>::infinity( ), -numeric_limits<float>::infinity( ));
  for( meshIt = m_meshes.begin(); meshIt!=m_meshes.end(); ++meshIt)
  {
    Vector3 mesh = meshIt->getBoundingBoxMax();
    bb_max.x = max(mesh.x, bb_max.x);
    bb_max.y = max(mesh.y, bb_max.y);
    bb_max.z = max(mesh.z, bb_max.z);
  }

  vector<ScAnimMesh*> :: const_iterator animMeshIt;
  for( animMeshIt = m_animMeshes.begin(); animMeshIt!=m_animMeshes.end(); ++animMeshIt)
  {
    Vector3 mesh = (*animMeshIt)->getBoundingBoxMax();
    bb_max.x = max(mesh.x, bb_max.x);
    bb_max.y = max(mesh.y, bb_max.y);
    bb_max.z = max(mesh.z, bb_max.z);
  }
  return bb_max;
}

void ScScene::readSceneParameters( string rt4FileName )
{
  FILE *file;
  char buffer[1024];

  file = fopen(rt4FileName.c_str(), "rt");
  MyAssert("File Not Found: " + rt4FileName, file);
  cout << "Reading Scene Parameters: " << rt4FileName << "...\n" << endl;

  int numScene = 0;

  while(!feof(file))
  {
    fscanf(file, "%s", buffer);
    if(buffer[0] == '#' || (buffer[0] == '/' && buffer[1] == '/') ) 
    {
      /* Ignore Coment */
      fscanf(file, "%*[^\n]s");
    }
    else if(!strcmp(buffer, "RT"))
    {
      /* Ignore File Version Information */
      fscanf(file, "%*[^\n]s");
    }else if(!strcmp(buffer, "SCENE"))
    {
      numScene++;
      if(numScene > 1)
      {
        fscanf(file, "%*[^\n]s");
        cout << "Cenas Multiplas definidas!" <<endl;
      }else
      {
        fscanf(file, "%[^\n]s", buffer);
        readFromStr(buffer);
      }
      break;
    }else
    {
      fscanf(file, "%*[^\n]s");
    }
  }
  fclose( file );

  assert(numScene == 1);
  configure();
}

void ScScene::readSceneObjects( string rt4FileName )
{
  FILE *file;
  char buffer[32768]; //32KB

  file = fopen(rt4FileName.c_str(), "rt");
  MyAssert("File Not Found: " + rt4FileName, file);
  cout << "Reading Scene Objects: " << rt4FileName << "...\n" << endl;

  int numCameras = 0;
  int numMaterials = 0;
  int numMeshes = 0;
  int numAnimMeshes = 0;
  int numLights = 0;

  while(!feof(file))
  {
    fscanf(file, "%s", buffer);
    if(buffer[0] == '#' || (buffer[0] == '/' && buffer[1] == '/') ) 
    {
      /* Ignore Coment */
      fscanf(file, "%*[^\n]s");
    }
    else if(!strcmp(buffer, "RT"))
    {
      /* Ignore File Version Information */
      fscanf(file, "%*[^\n]s");
    }else if(!strcmp(buffer, "CAMERA"))
    {
      numCameras++;
      fscanf(file, "%[^\n]s", buffer);

      ScCamera c;
      c.readFromStr(buffer);
      m_cameras.push_back(c);
    }else if(!strcmp(buffer, "MATERIAL"))
    {
      numMaterials++;
      fscanf(file, "%[^\n]s", buffer);

      ScMaterial m;
      m.readFromStr(buffer);
      m_materials.push_back(m);
    }else if(!strcmp(buffer, "LIGHT"))
    {
      numLights++;
      if(numLights > ScLight::Max_Lights)
      {
        fscanf(file, "%*[^\n]s");
        cout << "Maximo de Luzes definidas!" <<endl;
      }else
      {
        fscanf(file, "%[^\n]s", buffer);
        ScLight l;
        l.readFromStr(buffer);
        m_lights.push_back(l);
      }
    }else if(!strcmp(buffer, "MESH"))
    {
      numMeshes++;
      fscanf(file, "%[^\n]s", buffer);
      ScMesh m;
      m.readFromStr(buffer);
      if(m.getVbo() || m.getP3bMesh())
        m_meshes.push_back(m);
    }else if(!strcmp(buffer, "ANIMMESH"))
    {
      numAnimMeshes++;
      fscanf(file, "%[^\n]s", buffer);
      ScAnimMesh *m = new ScAnimMesh();
      m->readFromStr(buffer);
      m->configure();
      m_animMeshes.push_back(m);
    }else
    {
      fscanf(file, "%*[^\n]s");
    }
  }
  fclose( file );
  
  m_calculated = false;
  configure();
}

void ScScene::readScene( string rt4FileName )
{
  readSceneParameters(rt4FileName);
  readSceneObjects(rt4FileName);
  configure();
}

unsigned int ScScene::getNumVertices() const
{
  return m_numVertices;
}

unsigned int ScScene::getNumElements() const
{
  return m_numElements;
}

