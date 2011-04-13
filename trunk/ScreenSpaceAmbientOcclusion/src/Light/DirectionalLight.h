/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
                  Matricula:2610166
\**********************************************************/
/**********************************************************\
                  Cabecalho DirectionalLight.h
   Contem a definição da classe DirectionalLight
\**********************************************************/
#ifndef _DIRECTIONAL_LIGHT_H_
#define _DIRECTIONAL_LIGHT_H_

#include <string>

#include "Light/Light.h"
#include "GraphBasis/Vector3.h"

using namespace std;
class GraphObject3D;

class DirectionalLight : public Light
{
   public:
      DirectionalLight();
      ~DirectionalLight();

      virtual void render();
      virtual void configure();
};


#endif





