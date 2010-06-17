#include <iostream>
#include <GL\glew.h>
#include <GL\freeglut.h>

#include "GraphBasis\Vector3.h"
#include "GraphBasis\Shader.h"

#include "Objects\Scene.h"
#include "Kernels\KernelGeometry.h"

#include "main.h"

//Interface control
int lastMousePosX = 0;
int lastMousePosY = 0;
int mouseState = GLUT_UP;
int mouseButton = GLUT_RIGHT_BUTTON;

//Camera Position
float camAlpha = 0.0;
float camBeta = 40.0;
float camR = 300.0;

//Camera Attributes
float nearPlane = 0.1f;
float farPlane = 10000.0f;
float fov = 60.0f;

//Global Objects
Scene* rtScene;
KernelGeometry* kernelGeometry;

 GLenum e;
int main(int argc, char *argv[]){
	init(argc, argv);
	glewInit();	

	glutMainLoop();
}



void init(int argc, char *argv[]){
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(appWidth, appHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(APP_NAME);

  glutDisplayFunc(display);
	glutIdleFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboardSpecial);
	glutMotionFunc(mouseActive);
	glutMouseFunc(mouseButtons);
	glutReshapeFunc(reshape);
  
  glClearColor(0.8, 0.8, 0.8, 1.0);

  glShadeModel(GL_SMOOTH);
  glPolygonMode(GL_FRONT, GL_FILL);
  //   glPolygonMode(GL_BACK, GL_LINE);
  //   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);

  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //glEnable(GL_BLEND);

  glEnable(GL_DEPTH_TEST);
  //glEnable(GL_ALPHA_TEST);
  
	glEnable(GL_TEXTURE);
 // glEnable(GL_TEXTURE_1D);
  glEnable(GL_TEXTURE_2D);

  // disables cursor
  //   glutSetCursor(GLUT_CURSOR_NONE);
  e = glGetError();
  createScenes();
  e = glGetError();
}


void reshape(int w, int h){
	glViewport (0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective(fov, (GLfloat)w / (GLfloat)h, nearPlane, farPlane);
  glMatrixMode (GL_MODELVIEW);

  appWidth = w;
  appHeight = h;
}

void keyboardSpecial(int key, int x, int y){
	/*	
	if( key == GLUT_KEY_LEFT ) angleY+=ANGLE_STEP;
	else if( key == GLUT_KEY_RIGHT ) angleY-=ANGLE_STEP;
	else if( key == GLUT_KEY_UP ) angleX+=ANGLE_STEP;
	else if( key == GLUT_KEY_DOWN ) angleX-=ANGLE_STEP;
	*/
  //cout << key<<endl;
}

void keyboard(unsigned char key, int x, int y){
  switch(key)
  {
    case 27://ESC
      exit(42);
    break;
  }
  //cout << (int)key<<endl;
}

void mouseButtons(int button, int state, int x, int y){
	
	mouseState = state;
	mouseButton = button;

	lastMousePosX = x;
	lastMousePosY = y;
}

void mouseActive(int x, int y){
	if(mouseButton == GLUT_LEFT_BUTTON && mouseState == GLUT_DOWN){
		float angleX = (x - lastMousePosX);
		float angleY = (y - lastMousePosY);


		camAlpha = ((int)(camAlpha + angleX))%360;
		camBeta = ((int)(camBeta + angleY))%360;
	}
	else if(mouseButton == GLUT_RIGHT_BUTTON && mouseState == GLUT_DOWN){
		camR += (y - lastMousePosY)/50.0;
	}
	lastMousePosX = x;
	lastMousePosY = y;
}

void display()
{
  e = glGetError();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    e = glGetError();
  render();
  //glFlush();
  glutSwapBuffers();
  e = glGetError();
}

void renderAxis(){
  glBegin(GL_LINES);
  glColor3f(0, 0, 1); glVertex3f(0, 0, 0); glColor3f(0, 0, 1); glVertex3f(0, 0, 1000);
  glColor3f(0, 1, 0); glVertex3f(0, 0, 0); glColor3f(0, 1, 0); glVertex3f(0, 1000, 0);
  glColor3f(1, 0, 0); glVertex3f(0, 0, 0); glColor3f(1, 0, 0); glVertex3f(1000, 0, 0);
  glEnd();
}



void renderScreenQuad()
{
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glViewport(0, 0,  appWidth , appHeight);
  gluOrtho2D(0, 1, 0, 1);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
    glBegin(GL_QUADS);
      glTexCoord2d(0,0);
      glVertex2d(0,0);
      glTexCoord2d(1,0);
      glVertex2d(1,0);
      glTexCoord2d(1,1);
      glVertex2d(1,1);
      glTexCoord2d(0,1);
      glVertex2d(0,1);

      //glTexCoord2d(0,0);
      //glVertex3d(-1000,-1000,-100);
      //glTexCoord2d(1,0);
      //glVertex3d(1000,-1000,-100);
      //glTexCoord2d(1,1);
      //glVertex3d(1000,1000,-100);
      //glTexCoord2d(0,1);
      //glVertex3d(-1000,1000,-100);
    glEnd();
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}
#include"Light/PointLight.h"
PointLight p;
void createScenes()
{
  rtScene = new Scene("./resources/scenes/cavalo.rt4");
  //rtScene->setLightEnabled(false);
  p.setPosition(Vector3(0,10,0));
  p.setAmbientColor(Color(.2, .2, .2));
  p.setDiffuseColor(Color(.8,.8,.8));
  p.setSpecularColor(Color(1,1,1));

  kernelGeometry = new KernelGeometry(appWidth, appHeight);
}

void render(){
  float x = camR*sin(DEG_TO_RAD(camBeta))*cos(DEG_TO_RAD(camAlpha));
  float y = camR*sin(DEG_TO_RAD(camAlpha));
  float z = camR*cos(DEG_TO_RAD(camBeta))*cos(DEG_TO_RAD(camAlpha));
  
  glLoadIdentity();
  gluLookAt(x,y,z, 0, 0, 0, 1, 0, 0);

  //renderAxis();

  rtScene->configure();
/*
  glCullFace(GL_BACK);
  //glEnable(GL_CULL_FACE);
 //glEnable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
  //glDepthFunc(GL_LEQUAL);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_TEXTURE_2D);
  glColor4f(1,1,1,1);
  glClearColor(.8,.8,.8,1.);
  glClearDepth(1.0);
  glColorMask(true, true, true, true);
  glDepthMask(true);
  glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);


  glColorMask(false, false, false, false);
  glDepthMask(true);

  
  //renderScreenQuad();
  rtScene->render();
  glutSolidTeapot(60);



  glColorMask(true, true, true, true);
  glDepthMask(false);
*/

  //glCullFace(GL_BACK);
  //glCullFace(GL_FRONT);
  //glDisable(GL_CULL_FACE);
  //glEnable(GL_COLOR_MATERIAL);



 
  kernelGeometry->setActive(true);

  //p.configure();
  //p.render();
  glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
    rtScene->render();

  //glColorMaterial(GL_FRONT, GL_AMBIENT);
  //glColor3f(.2,0,0);
  //glColorMaterial(GL_FRONT, GL_DIFFUSE);
  //glColor3f(.8,0,0);
  //glColorMaterial(GL_FRONT, GL_SPECULAR);
  //glColor3f(1,0,0);

  //glutSolidTeapot(60);

  kernelGeometry->setActive(false);
  kernelGeometry->renderOutput(KernelGeometry::Position);

  /**/
}