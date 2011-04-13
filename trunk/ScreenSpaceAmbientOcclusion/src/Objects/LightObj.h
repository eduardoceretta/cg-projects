/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#pragma once

#include <string>
#include "assert.h"

#include "GraphBasis/Vector3.h"
#include "GraphBasis/Color.h"


#include "Light/PointLight.h"



using namespace std;

///lightStruct
//{
//0|   ambient  //alpha == enable or disable 
//1|   diffuse   //alpha == spotExponent
//2|   specular //
//3|   pos      //w == type :0 directional, 1 point, 2 = spot
//4|   spot     //rgb == spotDir, a == spotAngle(rad)
//}

struct lightStruct
{
  GLfloat diffuse[3];
  GLfloat spotExponent;
  GLfloat specular[3];
  GLfloat enabled;
  GLfloat pos[3];
  GLfloat type;
  GLfloat spotDir[3];
  GLfloat spotAngle;
};

class LightObj
{
   static int lightNum;
   enum LightType {Directional = 0, Point = 1, Spot = 2 };

   protected:
      PointLight mPLight;

      bool mCalculed;

      Vector3 mPos;
      Color mSpecular;
      Color mDiffuse;

      LightType mType;
      Vector3 mSpotDir;
      float mSpotExponent;
      float mSpotAngle;

      int myLightNum;

      struct lightStruct * mLightStruct;
   public:
      enum {Max_Lights = 8};

      LightObj();
      ~LightObj();

      int getMyLightNumber() const;
      void readFromStr(char buffer[]);

      void configure();
      void render();

      struct lightStruct * getLightStruct();

};

