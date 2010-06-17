/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/

#include <iostream>

#include "Objects/LightObj.h"

using namespace std;

int LightObj :: lightNum = 0;
///////////////////
//~ Light
//////////////////
LightObj :: LightObj()
:mCalculed(false)
,mLightStruct(NULL)
{
   myLightNum = lightNum++;

   if(myLightNum >= Max_Lights)
      myLightNum = myLightNum%Max_Lights;
}

LightObj :: ~LightObj()
{
  if(mLightStruct)
    delete mLightStruct;
}

int LightObj :: getMyLightNumber() const
{
   return myLightNum;
}


void LightObj :: readFromStr(char buffer[])
{
   int r = sscanf( buffer, "%f %f %f %f %f %f\n", &mPos.x, &mPos.y, &mPos.z,
      &mDiffuse.r, &mDiffuse.g, &mDiffuse.b);
   mDiffuse *= 1.0/255;
   mSpecular = mDiffuse;
   assert(r == 6);
//   cout << "Light num, pos, Diffuse, Specular:\n"<<myLightNum << endl << mPos  << mDiffuse << mSpecular <<endl;
}

void LightObj :: configure()
{
   if(!mCalculed)
   {
      mPLight.setAmbientColor(mDiffuse*.2);
      mPLight.setDiffuseColor(mDiffuse);
      mPLight.setSpecularColor(mSpecular);
      mPLight.setPosition(mPos);

      

      mCalculed = true;
   }
   mPLight.configure();
}

void LightObj :: render()
{
   mPLight.render();
}

struct lightStruct * LightObj::getLightStruct()
{
  if(mLightStruct)
    return mLightStruct;

  mLightStruct = new lightStruct;
  mLightStruct->diffuse[0] = mDiffuse.r;
  mLightStruct->diffuse[1] = mDiffuse.g;
  mLightStruct->diffuse[2] = mDiffuse.b;
  mLightStruct->spotExponent = 0.0;
  mLightStruct->specular[0] = mSpecular.r;
  mLightStruct->specular[1] = mSpecular.g;
  mLightStruct->specular[2] = mSpecular.b;
  mLightStruct->enabled = 1.0;
  mLightStruct->pos[0] = mPos.x;
  mLightStruct->pos[1] = mPos.y;
  mLightStruct->pos[2] = mPos.z;
  mLightStruct->type = 1.0;
  mLightStruct->spotDir[0] = 1;
  mLightStruct->spotDir[1] = 0;
  mLightStruct->spotDir[2] = 1;
  mLightStruct->spotAngle = 3.1415;
  return mLightStruct;
}



