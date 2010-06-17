/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#pragma once

#include <string>
#include "assert.h"

#include "GraphBasis/Vector3.h"

using namespace std;

class Camera
{
   protected:
      Vector3 mPos;
      Vector3 mAt;
      Vector3 mUp;
      float mFovy;
      float mNear;
      float mFar;
      int mScreenWidth;
      int mScreenHeight;

   public:
      Camera(){}
      Camera(Vector3 pos, Vector3 at, Vector3 up, float fovy,
         float near, float far, int screenWidth, int screenHeight);
      void readFromStr(char buffer[]);
};




