/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#pragma once
#include <string>
#include "assert.h"
#include <GL/glut.h>

#include "GraphBasis/Vector3.h"
#include "GraphBasis/Color.h"

using namespace std;

class Material
{
   static int materialNum;


   protected:
      bool mCalculed;

      GLfloat mGAmbient[4];
      GLfloat mGSpecular[4];
      GLfloat mGDiffuse[4];
      GLfloat mGShi;

      
      
      float mReflective;
      float mRefractive;
      float mOpacity;

      int myMaterialNum;
   public:
      Material();

      int getMyMaterialNumber() const;
      void readFromStr(char buffer[]);

      void configure();
      void render();

	  Color mSpecular;
      Color mDiffuse;
	  float mSpecularExp;

};

