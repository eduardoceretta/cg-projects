/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
                  Matricula:2610166
\**********************************************************/
/**********************************************************\
                  Cabecalho Light.h
   Contem a definição da classe Light
\**********************************************************/
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <string>
#include <GL/glut.h>

#include "GraphBasis/Vector3.h"
#include "GraphBasis/Color.h"

using namespace std;

class Light
{
   static int lightNum;

   protected:
      GLfloat pos[4];
      GLfloat ambient[4];
      GLfloat specular[4];
      GLfloat diffuse[4];

      double constAtenuation;
      double linAtenuation;
      double quadAtenuation;

      bool turnedOn;
      bool modified;

      int myLightNum;
   public:
      Light();
      virtual ~Light() = 0;

      virtual void render();
      virtual void configure();

      void setPosition(Vector3 pos);
      Vector3 getPosition() const;

      void setAmbientColor(Color c);
      Color getAmbientColor() const;

      void setSpecularColor(Color c);
      Color getSpecularColor() const;

      void setDiffuseColor(Color c);
      Color getDiffuseColor() const;

      void lightTurnedOn(bool op); //Luz Ligada ou desligada
      bool lightIsTurnedOn() const;


      void setAtenuation(double constant, double linear, double quadric); //Atenuação da luz
      void getAtenuation(double &constant, double &linear, double &quadric) const;

      int getMyLightNumber() const;
};



#endif



