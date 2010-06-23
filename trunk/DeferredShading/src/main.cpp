#include <iostream>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <cmath>

#include "GraphBasis\Vector3.h"
#include "GraphBasis\Shader.h"

#include "Objects\Scene.h"
#include "Kernels\KernelGeometry.h"
#include "Kernels\KernelShade.h"

#include "main.h"

//Interface control
int lastMousePosX = 0;
int lastMousePosY = 0;
int mouseState = GLUT_UP;
int mouseButton = GLUT_RIGHT_BUTTON;

bool enabled = false;

//Camera Position
float camAlpha = 0.0;
float camBeta = 40.0;
float camR = 300.0;
float camInc = 5.0;

//Camera Attributes
float nearPlane = 0.1f;
float farPlane = 10000.0f;
float fov = 60.0f;

//Global Objects
Scene* rtScene;
KernelGeometry* kernelGeometry;
KernelShade* kernelShade;

//Debug
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
  //glEnable(GL_CULL_FACE);

  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //glEnable(GL_BLEND);

  glEnable(GL_DEPTH_TEST);
  //glEnable(GL_ALPHA_TEST);
  
	//glEnable(GL_TEXTURE);
 // glEnable(GL_TEXTURE_1D);
  //glEnable(GL_TEXTURE_2D);

  // disables cursor
  //   glutSetCursor(GLUT_CURSOR_NONE);
  createScenes();
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


static bool filled = true;
void keyboardSpecial(int key, int x, int y){

	if( key == GLUT_KEY_LEFT ) camBeta = (int)(camBeta + camInc)%360;
	else if( key == GLUT_KEY_RIGHT )  camBeta = (int)(camBeta - camInc)%360;
	else if( key == GLUT_KEY_UP ) camAlpha = (int)(camAlpha - camInc)%360;
	else if( key == GLUT_KEY_DOWN )camAlpha = (int)(camAlpha + camInc)%360;

  switch(key)
  {
  case 10: //F10
    if(filled)
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
       glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    filled = !filled;
    break;
  }

  
}

void keyboard(unsigned char key, int x, int y){
  switch(key)
  {
    case 27://ESC
      exit(42);
    break;
    case 'q':
    case 'Q':
      enabled = !enabled;
      if(enabled)
        cout << "Shader ON"<<endl;
      else cout << "Shader OFF"<<endl;
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
		float angleX = (x - lastMousePosX)*.5;
		float angleY = (y - lastMousePosY)*.5;


		camAlpha = ((int)(camAlpha + angleY))%360;
		camBeta = ((int)(camBeta + angleX))%360;
	}
	else if(mouseButton == GLUT_RIGHT_BUTTON && mouseState == GLUT_DOWN){
		camR += (y - lastMousePosY)/2.0;
	}
	lastMousePosX = x;
	lastMousePosY = y;
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  render();
  //glFlush();
  glutSwapBuffers();
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

#include "Light/PointLight.h"
#include "Light/SpotLight.h"
#include "Light/DirectionalLight.h"
PointLight p;
PointLight p2;
SpotLight sp2;
DirectionalLight d;
void createScenes()
{
  rtScene = new Scene("./resources/scenes/cavalo.rt4");
  //rtScene->setLightEnabled(false);

  p.setAmbientColor(Color(0.0,0.0,0.0));
  p.setDiffuseColor(Color(.8,.8,.8));
  p.setSpecularColor(Color(1.,1.,1.));
  p.setPosition(Vector3(0,100,0));

  p2.setAmbientColor(Color(0.0,0.0,0.0));
  p2.setDiffuseColor(Color(.8,.8,.8));
  p2.setSpecularColor(Color(1.,1.,1.));
  p2.setPosition(Vector3(100,0,0));

  d.setAmbientColor(Color(0.0,0.0,0.0));
  d.setDiffuseColor(Color(.8,.8,.8));
  d.setSpecularColor(Color(1.,1.,1.));
  d.setPosition(Vector3(0,-1,0));


  sp2.setAmbientColor(Color(0.0,0.0,0.0));
  sp2.setDiffuseColor(Color(.8,.8,.8));
  sp2.setSpecularColor(Color(1.,1.,1.));
  sp2.setPosition(Vector3(0,0,100));
  sp2.setSpotExponent(1.0);
  sp2.setSpotAngle(30);
  sp2.setSpotDirection(Vector3(0,0,-1));

  GLfloat amb [] = {0.2,0.2,0.2,1};
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
  


  kernelGeometry = new KernelGeometry(appWidth, appHeight);
  kernelShade = new KernelShade(appWidth, appHeight, 
      kernelGeometry->getTexIdPosition(), 
      kernelGeometry->getTexIdNormal(), 
      kernelGeometry->getTexIdDiffuse(), 
      kernelGeometry->getTexIdSpecular(), 
      rtScene->getLightsTexId(),
      rtScene->getLightsTexSize());
}



void render(){
  float x = camR*sin(DEG_TO_RAD(camBeta))*cos(DEG_TO_RAD(camAlpha));
  float y = camR*sin(DEG_TO_RAD(camAlpha));
  float z = camR*cos(DEG_TO_RAD(camBeta))*cos(DEG_TO_RAD(camAlpha));

  float nextAlpha =  min(camAlpha + camInc,360.0f);

  float ux = sin(DEG_TO_RAD(camBeta))*cos(DEG_TO_RAD(nextAlpha)) - x;
  float uy = sin(DEG_TO_RAD(nextAlpha)) - y;
  float uz = cos(DEG_TO_RAD(camBeta))*cos(DEG_TO_RAD(nextAlpha)) - z;
  
  glLoadIdentity();
  gluLookAt(x,y,z, 0, 0, 0, ux, uy, uz);
  
  GLfloat lightModelViewMatrix[16];
  glGetFloatv(GL_MODELVIEW_MATRIX, lightModelViewMatrix);


  rtScene->configure();

  
  if(enabled)kernelGeometry->setActive(true);
  if(enabled)glClearColor(.8, .8, .8, -1.0);
  if(enabled)glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

  if(!enabled)
  {
    p.configure();
    //p2.configure();
    sp2.configure();
      d.configure();

    p.render();
    //p2.render();
    sp2.render();
    d.render();

  }else 
  {
    glDisable(GL_LIGHTING);
  }

  //rtScene->render();
  glPushMatrix();

  //glScalef(10.,1.,10.);
  
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
  glColor3f(1,1,1);
  glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
  glColor3f(1,0,0);
  glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
  glColor3f(1,1,1);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10.0);
  
  glCullFace(GL_FRONT);
  glutSolidTeapot(60);
  glCullFace(GL_BACK);

  glutSolidSphere(60, 30, 30);
  //glutSolidSphere(60, 500, 500);
  
  //glBegin(GL_QUADS);
  //  glNormal3f(0,-1,0);
  //  glVertex3f(-1000,300,-1000);
  //  glVertex3f(1000 ,300,-1000);
  //  glVertex3f(1000 ,300,1000);
  //  glVertex3f(-1000,300,1000);
  //glEnd();

  glPopMatrix();
  if(enabled)kernelGeometry->setActive(false);
  //if(enabled)kernelGeometry->renderOutput(KernelGeometry::Specular);

  if(enabled) kernelShade->renderShader(lightModelViewMatrix);

  /*
  LIGHT 0. 0. 130.     80. 250. 250.
  LIGHT 250. 0. 0.     150. 1. 10.


  /**/
}
