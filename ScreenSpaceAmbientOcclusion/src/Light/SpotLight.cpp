/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
                  Matricula:2610166
\**********************************************************/
/**********************************************************\
                  Arquivo SpotLight
   Contem a implementaçao dos metodos da classe SpotLight.cpp
\**********************************************************/

#include <iostream>

#include "Light/SpotLight.h"

using namespace std;

///////////////////
//~ SpotLight
//////////////////
SpotLight :: SpotLight():Light()
{
   memset(direction,0,3*sizeof(GLfloat));
   pos[3] = 1.0;
   cutOfAng = 45.0;
   exponentValue = 0.0;
}

SpotLight :: ~SpotLight()
{
//      cout << "DESTRUCTOR "<<_class << " " << _name<< endl;
//   cout << "END DESTRUCTOR "<<_class << " " << _name<< endl;
}


void SpotLight :: render()
{
   glLightfv(myLightNum, GL_SPOT_DIRECTION, direction);
   Light :: render();
}

void SpotLight :: configure()
{
   Light::configure();
   glLightf(myLightNum, GL_SPOT_CUTOFF, cutOfAng);
   glLightf(myLightNum, GL_SPOT_EXPONENT, exponentValue);
}


void SpotLight :: setSpotAngle(double ang)
{
   cutOfAng = ang;
   modified = true;
}

double SpotLight :: getSpotAngle()const
{
   return cutOfAng;
}

void SpotLight :: setSpotExponent(double exp)
{
   exponentValue = exp;
   modified = true;
}

double SpotLight :: getSpotExponent() const
{
   return exponentValue;
}

void SpotLight :: setSpotDirection(Vector3 dir)
{
   direction[0] = dir.x;
   direction[1] = dir.y;
   direction[2] = dir.z;
   modified = true;
}

Vector3 SpotLight :: getSpotDirection() const
{
   return Vector3(direction[0],direction[1],direction[2]);
}

