/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
                  Matricula:2610166
\**********************************************************/
/**********************************************************\
                  Cabecalho SpotLight.h
   Contem a definição da classe SpotLight
\**********************************************************/
#ifndef _SPOT_LIGHT_H_
#define _SPOT_LIGHT_H_

#include <string>

#include "Light/Light.h"
#include "GraphBasis/Vector3.h"

using namespace std;

class SpotLight : public Light
{
   protected:
      GLfloat direction[3];
      double cutOfAng;  //Angulo de abertura [0...90]
      double exponentValue; //Valor de atenuação nas bordas. Qnt maior mais focada eh o centro da luz
   public:
      SpotLight();
      ~SpotLight();

      virtual void render();
      virtual void configure();

      void setSpotAngle(double ang);
      double getSpotAngle()const;

      void setSpotExponent(double exp);
      double getSpotExponent()const;

      void setSpotDirection(Vector3 dir);
      Vector3 getSpotDirection()const;
};


#endif





