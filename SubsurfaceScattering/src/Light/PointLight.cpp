/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
                  Matricula:2610166
\**********************************************************/
/**********************************************************\
                  Arquivo PointLight
   Contem a implementaçao dos metodos da classe PointLight.cpp
\**********************************************************/

#include <iostream>
#include <GL/glut.h>

#include "Light/PointLight.h"



using namespace std;

///////////////////
//~ PointLight
//////////////////
PointLight :: PointLight():Light()
{
   pos[3] = 1.0;
}

PointLight :: ~PointLight()
{
//      cout << "DESTRUCTOR "<<_class << " " << _name<< endl;
//   cout << "END DESTRUCTOR "<<_class << " " << _name<< endl;
}


void PointLight :: render()
{
   Light :: render();
}

void PointLight :: configure()
{
   Light :: configure();
}
