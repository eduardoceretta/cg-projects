/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#pragma once

#include <vector>
#include <string>
#include "string.h"

#include "GraphBasis/Vector3.h"
#include "GraphBasis/Color.h"

#include "Objects\Camera.h"
#include "Objects\LightObj.h"
#include "Objects\Mesh.h"
#include "Objects\Material.h"
#include "Objects\Triangle.h"

using namespace std;


class Scene
{
  protected:
      bool m_calculated;
      Color mClear;
      Color mAmbient;

      vector<Material> mMaterials;
      vector<LightObj> mLights;
      vector<Mesh> mMeshes;
      Camera mCamera;

      bool m_lightEnabled;
      GLuint mLightsTexId;;

   public:
	    Scene(){};
      Scene(string rt4FileName);
      ~Scene();

      void configure();
      void render();

      GLuint getLightsTexId();
      GLfloat getLightsTexSize();
      int getNumLights();
      int getNumMeshes();
      Mesh* getMeshAt(int i);
      unsigned int getSceneNumTriangles();

      bool isLightEnabled() const;
      void setLightEnabled(bool val);
      void setLightActive(bool op);
      void setMaterialActive(bool op, int index);
      void renderMesh(int index);



protected:
      void readFromStr(char buffer[]);
      void readFromFile(string rt4FileName);


      void calcTextures();
};

