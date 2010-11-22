/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "main.h"
#include "Objects/Scene.h"

using namespace std;

///////////////////
//~ Scene
//////////////////
Scene :: Scene(string rt4FileName)
:mCalculed(false)
,m_lightEnabled(true)
{
  readFromFile(rt4FileName);
  configure();
}

Scene :: ~Scene()
{
   GLuint id[] = {
    mLightsTexId
  };
  glDeleteTextures(sizeof(id)/sizeof(GLuint), id);
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
//	   cout << buffer <<endl;
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
            mCamera.readFromStr(buffer);
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
         Material m;
         m.readFromStr(buffer);
         mMaterials.push_back(m);
		}else if(!strcmp(buffer, "LIGHT"))
		{
         numLights++;
         if(numScene > LightObj::Max_Lights)
         {
            fscanf(file, "%*[^\n]s");
            cout << "Maximo de Luzes definidas!" <<endl;
         }else
         {
            fscanf(file, "%[^\n]s", buffer);
            LightObj l;
            l.readFromStr(buffer);
            mLights.push_back(l);
         }
      }else if(!strcmp(buffer, "MESH"))
		{
         numMeshes++;
         fscanf(file, "%[^\n]s", buffer);
         Mesh m;
         m.readFromStr(buffer);
         mMeshes.push_back(m);
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
   int r = sscanf( buffer, "%f %f %f %f %f %f %*s\n", &mClear.r, &mClear.g, &mClear.b,
      &mAmbient.r, &mAmbient.g, &mAmbient.b); //,* == backgroundFileName
   assert(r == 7-1);//-1 == backgroundFileName

//   cout <<"Scene Clear, Ambient" << mClear  << mAmbient<<endl;
}

void Scene :: configure()
{
  if(!mCalculed) 
  {
    calcTextures();
    mCalculed = true;
  }
  if(m_lightEnabled)
  {
    vector<LightObj> :: iterator lightIt;
    for( lightIt = mLights.begin(); lightIt!=mLights.end(); ++lightIt)
    {
      lightIt->configure();
    }

    vector<Mesh> :: iterator meshIt;
    for( meshIt = mMeshes.begin(); meshIt!=mMeshes.end(); ++meshIt)
    {
      mMaterials[meshIt->getMaterialIndex()].configure();
      meshIt->configure();
    }
	
  }
}


void Scene :: render()
{
  glPushAttrib(GL_LIGHTING_BIT);

    if(m_lightEnabled)
    {
      vector<LightObj> :: iterator lightIt;
      for( lightIt = mLights.begin(); lightIt!=mLights.end(); ++lightIt)
        lightIt->render();
    }

    vector<Mesh> :: iterator meshIt;
    for( meshIt = mMeshes.begin(); meshIt!=mMeshes.end(); ++meshIt)
    {
      glPushAttrib(GL_LIGHTING_BIT);
        //if(m_lightEnabled)  
          mMaterials[meshIt->getMaterialIndex()].render();
        //else mMaterials[meshIt->getMaterialIndex()].mDiffuse.setColor();
        meshIt->render();
      glPopAttrib();
    }
  glPopAttrib();
}


void Scene::calcTextures()
{
  GLenum sizeType [] = {0,GL_ALPHA, 2 ,GL_RGB, GL_RGBA};
  
  GLfloat* lData = new GLfloat[mLights.size()*sizeof(struct lightStruct)];
  for( unsigned int i = 0; i < mLights.size(); ++i)
    memcpy(&lData[i*sizeof(struct lightStruct)/sizeof(GLfloat)], mLights[i].getLightStruct(), sizeof(struct lightStruct));

  int lightArraySize = mLights.size()*sizeof(struct lightStruct)/(4*sizeof(GLfloat));
  int lightArrayAbsoluteSize = mLights.size()*sizeof(struct lightStruct)/(sizeof(GLfloat));


  GLfloat* data[] = { lData/*RGBA*/ };
  
  
  unsigned int size[] = { lightArraySize };
  
  int sizeIndex [] = { lightArrayAbsoluteSize/lightArraySize };
  
  int numTextures = sizeof(data)/sizeof(GLfloat*);
  GLuint *id = new GLuint[numTextures];
  //// //DEBUG
  //char sizeTypeStr [5][10] = {"NOT","GL_ALPHA", "2" ,"GL_RGB", "GL_RGBA"};
  //for(int i=0;i<numTextures;++i)
  //{
  //  printf("%d %s\n",sizeIndex[i], sizeTypeStr[sizeIndex[i]]);
  //}

  glGenTextures(numTextures, id);
  for(int i=0; i<numTextures; ++i)
  {
    glBindTexture(GL_TEXTURE_1D, id[i]);
    //   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F_ARB, size[i], 0, sizeType[sizeIndex[i]], GL_FLOAT, data[i]);
    glBindTexture(GL_TEXTURE_1D, 0);
  }
  
  int i = 0;
  mLightsTexId = id[i++];
  
  delete[] id;
}

GLuint Scene::getLightsTexId()
{
  return mLightsTexId;
}

GLfloat Scene::getLightsTexSize()
{
  return  mLights.size()*sizeof(struct lightStruct)/(4*sizeof(GLfloat));
}

bool Scene::isLightEnabled() const
{
  return m_lightEnabled; 
}

void Scene::setLightEnabled(bool val) 
{ 
  m_lightEnabled = val; 
}

int Scene::getNumLights()
{
  return mLights.size();
}

unsigned int Scene::getSceneNumTriangles()
{
  return Triangle::getMaxNumTriangles();
}

void Scene::setLightActive( bool op )
{
  if(op)
  {
    if(m_lightEnabled)
    {
      glPushAttrib(GL_LIGHTING_BIT);
     
      vector<LightObj> :: iterator lightIt;
      for( lightIt = mLights.begin(); lightIt!=mLights.end(); ++lightIt)
      {
        lightIt->configure();
        lightIt->render();
      }

    }
  }else {
    glPopAttrib();
  }
}