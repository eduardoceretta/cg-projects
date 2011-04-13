/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
                  Matricula:2610166
\**********************************************************/
/**********************************************************\
                  Cabecalho PointLight.h
   Contem a definição da classe PointLight
\**********************************************************/
#ifndef _POINT_LIGHT_H_
#define _POINT_LIGHT_H_

#include <string>
#include <GL/glut.h>

#include "Light/Light.h"
#include "GraphBasis/Vector3.h"

using namespace std;


class PointLight : public Light
{
   protected:
   public:
      PointLight();
      ~PointLight();

      virtual void render();
      virtual void configure();

};

#endif




