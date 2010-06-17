/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
                  Matricula:2610166
\**********************************************************/
/**********************************************************\
                  Arquivo Light
   Contem a implementaçao dos metodos da classe Light.cpp
\**********************************************************/

#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>

#include "Light/Light.h"

using namespace std;

int Light :: lightNum = 0;
#define MY_MAX_LIGHT 8
///////////////////
//~ Light
//////////////////
Light :: Light()
{
   myLightNum = (GL_LIGHT0+lightNum++);


   if(myLightNum >=(GL_LIGHT0+MY_MAX_LIGHT))
   {
      myLightNum = myLightNum%(GL_LIGHT0+MY_MAX_LIGHT)+GL_LIGHT0;
   }

   for (unsigned i(0); i < 4; ++i)
   {
      pos[i] = 0;
      ambient[i] = 0.1;
      specular[i] = 1;
      diffuse[i] = 1;
   }
   ambient[3] = 1;

   shi = 0.0;

//   constAtenuation = 2.0;
//   linAtenuation = 1.0;
//   quadAtenuation = .5;

   constAtenuation = 1.0;
   linAtenuation = 0.0;
   quadAtenuation = .0;

   turnedOn = true;
   modified = true;
}

Light :: ~Light()
{
//   cout << "DESTRUCTOR "<<_class << " " << _name<< endl;

   glDisable(myLightNum);

//   cout << "END DESTRUCTOR "<<_class << " " << _name<< endl;
}

void Light :: render()
{
   if(turnedOn)
   {
      glEnable(GL_LIGHTING);
      glEnable(myLightNum);
      glLightfv(myLightNum, GL_POSITION, pos); //Setada para que as transformaçoes acima sejam aplicadas a luz tb
   }
   else glDisable(myLightNum);
}
void Light :: configure()
{
   if(modified && turnedOn)
   {
      glLightfv(myLightNum, GL_AMBIENT,  ambient);
      glLightfv(myLightNum, GL_DIFFUSE,  diffuse);
      glLightfv(myLightNum, GL_SPECULAR, specular);

      glLightf(myLightNum, GL_CONSTANT_ATTENUATION, constAtenuation);
      glLightf(myLightNum, GL_LINEAR_ATTENUATION, linAtenuation);
      glLightf(myLightNum, GL_QUADRATIC_ATTENUATION, quadAtenuation);
      modified = false;
   }
}

void Light :: setPosition(Vector3 position)
{
   pos[0] = position.x;
   pos[1] = position.y;
   pos[2] = position.z;
   modified = true;
}

Vector3 Light :: getPosition() const
{
   return Vector3(pos[0],pos[1],pos[2]);
}


void Light :: setAmbientColor(Color c)
{
   ambient[0] = c.r;
   ambient[1] = c.g;
   ambient[2] = c.b;
   modified = true;
}

Color Light :: getAmbientColor() const
{
   return Color(ambient[0],ambient[1],ambient[2]);
}


void Light :: setSpecularColor(Color c)
{
   specular[0] = c.r;
   specular[1] = c.g;
   specular[2] = c.b;
   modified = true;
}

Color Light :: getSpecularColor() const
{
   return Color(specular[0],specular[1],specular[2]);
}


void Light :: setDiffuseColor(Color c)
{
   diffuse[0] = c.r;
   diffuse[1] = c.g;
   diffuse[2] = c.b;
   modified = true;
}

Color Light :: getDiffuseColor() const
{
   return Color(diffuse[0],diffuse[1],diffuse[2]);
}


void Light :: setShineValue(double shine)
{
   shi = shine;
   modified = true;
}

double Light :: getShineValue() const
{
   return shi;
}


void Light :: lightTurnedOn(bool op)
{
   turnedOn = op;
}

bool Light :: lightIsTurnedOn() const
{
   return turnedOn;
}


void Light :: setAtenuation(double constant, double linear, double quadric)
{
   constAtenuation = constant;
   linAtenuation = linear;
   quadAtenuation = quadric;
   modified = true;
}

void Light :: getAtenuation(double &constant, double &linear, double &quadric)  const
{
   constant = constAtenuation ;
   linear = linAtenuation;
   quadric = quadAtenuation;
}


int Light :: getMyLightNumber() const
{
   return myLightNum - GL_LIGHT0;
}



