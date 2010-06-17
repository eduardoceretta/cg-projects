/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#include <iostream>

#include "Objects/Material.h"

using namespace std;

int Material :: materialNum = 0;

///////////////////
//~ Material
//////////////////
Material :: Material()
:mCalculed(false)
{
   myMaterialNum = materialNum++;
}


int Material :: getMyMaterialNumber() const
{
   return myMaterialNum;
}

void Material :: readFromStr(char buffer[])
{
   int r = sscanf( buffer, "%f %f %f %f %f %f %f %f %f %f %*s\n", &mDiffuse.r, &mDiffuse.g, &mDiffuse.b, &mSpecular.r, &mSpecular.g, &mSpecular.b, &mSpecularExp, &mReflective, &mRefractive, &mOpacity);//,* == textureFileName );
   mDiffuse *= 1.0/255;
   mSpecular *= 1.0/255;
   assert(r == 11 - 1);//,-1 == textureFileName );
//   cout << "Material num, pos, Diffuse, Specular, SpecularExp, Reflective, Refractive, Opacity:\n"<< myMaterialNum << endl << mDiffuse << mSpecular << mSpecularExp << endl <<mReflective << endl << mRefractive << endl << mOpacity <<endl;
}

void Material :: configure()
{
   if(!mCalculed)
   {
      mGAmbient[0] = mDiffuse.r*.2;
      mGAmbient[1] = mDiffuse.g*.2;
      mGAmbient[2] = mDiffuse.b*.2;
      mGAmbient[3] = 1.0;

      mGDiffuse[0] = mDiffuse.r;
      mGDiffuse[1] = mDiffuse.g;
      mGDiffuse[2] = mDiffuse.b;
      mGDiffuse[3] = 1.0;

      mGSpecular[0] = mSpecular.r;
      mGSpecular[1] = mSpecular.g;
      mGSpecular[2] = mSpecular.b;
      mGSpecular[3] = 1.0;

      mGShi = mSpecularExp;
      mCalculed = true;
   }
}

void Material :: render()
{
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  mGAmbient);
   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  mGDiffuse);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mGSpecular);
   glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mGShi);
}
