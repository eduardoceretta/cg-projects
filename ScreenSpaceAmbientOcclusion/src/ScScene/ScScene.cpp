/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Mar 2010
 *
 *  Scene file loader and OpenGL scene container
 *  Load Meshes, Materials, Lights and Camera
 */
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>

#include "defines.h"
#include "ScScene/ScScene.h"

using namespace std;

Scene :: Scene(string rt4FileName)
:m_calculated(false)
,m_lightEnabled(true)
{
  readFromFile(rt4FileName);
  configure();
}

Scene :: ~Scene()
{
}

void Scene :: readFromFile(string rt4FileName)
{
	FILE *file;
	char buffer[1024];

	file = fopen(rt4FileName.c_str(), "rt");
  MyAssert("File Not Found: " + rt4FileName, file);

	int numScene = 0;
	int numCamera = 0;
	int numMaterials = 0;
	int numMeshes = 0;
	int numLights = 0;

	while(!feof(file))
	{
	   fscanf(file, "%s", buffer);
     if(buffer[0] == '#') 
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
         numCamera++;
         if(numCamera > 1)
         {
            fscanf(file, "%*[^\n]s");
            cout << "Cameras Multiplas definidas!" <<endl;
         }else
         {
            fscanf(file, "%[^\n]s", buffer);
            m_camera.readFromStr(buffer);
         }
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
         if(numScene > ScLight::Max_Lights)
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
		}else
		{
			printf( "Ignorando comando: %s\n", buffer );
         fscanf(file, "%*[^\n]s");
		}
	}
   fclose( file );

	assert(numCamera == 1);
	assert(numScene == 1);
}

void Scene :: readFromStr(char buffer[])
{
   int r = sscanf( buffer, "%f %f %f %f %f %f %*s\n", &m_clear.r, &m_clear.g, &m_clear.b,
      &m_ambient.r, &m_ambient.g, &m_ambient.b); 
   assert(r == 7-1);
}

void Scene :: configure()
{
  if(!m_calculated) 
  {
    m_calculated = true;
  }
  if(m_lightEnabled)
  {
    vector<ScLight> :: iterator lightIt;
    for( lightIt = m_lights.begin(); lightIt!=m_lights.end(); ++lightIt)
    {
      lightIt->configure();
    }

    vector<ScMesh> :: iterator meshIt;
    for( meshIt = m_meshes.begin(); meshIt!=m_meshes.end(); ++meshIt)
    {
      m_materials[meshIt->getMaterialIndex()].configure();
      meshIt->configure();
    }
  }
}


void Scene :: render()
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
  glPopAttrib();
}


bool Scene::isSceneLightEnabled() const
{
  return m_lightEnabled; 
}

void Scene::setSceneLightEnabled(bool val) 
{ 
  m_lightEnabled = val; 
}

int Scene::getNumLights()
{
  return m_lights.size();
}


void Scene::setLightActive( bool op )
{
  if(op)
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
    glPopAttrib();
  }
}

void Scene::setMaterialActive( bool op , int index)
{
  if(op)
  {
    glPushAttrib(GL_LIGHTING_BIT);
    m_materials[m_meshes[index].getMaterialIndex()].configure();
    m_materials[m_meshes[index].getMaterialIndex()].render();
  }else
  {
    glPopAttrib();
  }
}

void Scene::renderMesh(int index)
{
  m_meshes[index].configure();
  m_meshes[index].render();
}

int Scene::getNumMeshes()
{
  return m_meshes.size();
}

ScMesh* Scene::getMeshAt( int i )
{
  return &m_meshes[i];
}

Color Scene::getClearColor() const
{
  return m_clear;
}

Color Scene::getAmbientColor() const
{
  return m_ambient;
}

ScCamera* Scene::getCamera()
{
  return &m_camera;
}