/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Feb 2011
 *
 *  Test Application's Main code.
 *  Create the test application and bind the GLUT callbacks.
 */
#include <iostream>
#include <GL\glew.h>
#include <GL\glut.h>

#include "testMain.h"

#include "testApp.h"

TestApp ssaoApp;

int main(int argc, char *argv[]){

  ssaoApp.loadParameters(argc, argv);
  ssaoApp.initGL(&argc, argv);
  ssaoApp.loadResources();

  setGlutCallbacks();
  glutMainLoop();
}

void setGlutCallbacks(){
  glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(reshape);
}

void reshape(int w, int h){
  ssaoApp.listenReshape(w, h);
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  ssaoApp.render();

#ifndef NDEBUG  
  GLenum e = glGetError();
  if(e)
    printf("GL_ERROR\n");
#endif  

  glutSwapBuffers();
}


