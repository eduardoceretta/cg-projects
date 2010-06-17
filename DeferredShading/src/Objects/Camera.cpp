/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#include <iostream>

#include "Objects/Camera.h"

using namespace std;

///////////////////
//~ Camera
//////////////////
Camera :: Camera(Vector3 pos, Vector3 at, Vector3 up, float fovy,
         float near, float far, int screenWidth, int screenHeight)
:mPos(pos)
,mAt(at)
,mUp(up)
,mFovy(fovy)
,mNear(near)
,mFar(far)
,mScreenWidth(screenWidth)
,mScreenHeight(screenHeight)
{
}

void Camera :: readFromStr(char buffer[])
{
   int r = sscanf( buffer, "%f %f %f %f %f %f %f %f %f %f %f %f %d %d\n", &mPos.x, &mPos.y, &mPos.z,
      &mAt.x, &mAt.y, &mAt.z, &mUp.x, &mUp.y, &mUp.z, &mFovy, &mNear, &mFar, &mScreenWidth, &mScreenHeight);
   assert(r == 14);
//   cout << "Camera pos, at, up, fovy, near, far, screenW, screenH:\n"<<mPos << mAt << mUp << mFovy <<endl << mNear << endl << mFar << endl << mScreenWidth << endl << mScreenHeight <<endl;
}

