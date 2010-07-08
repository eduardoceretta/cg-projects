#include <iostream>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <cmath>

#include "GraphBasis\Vector3.h"
#include "GraphBasis\Shader.h"
#include "GraphBasis/GLFont.h"
#include "GraphBasis/Frames.h"

#include "Objects\Scene.h"
#include "Kernels\KernelGeometry.h"
#include "Kernels\KernelShade.h"
#include "Kernels\KernelAntiAliasing.h"
#include "Kernels\KernelAntiAliasingN.h"

#include "main.h"


int appWidth = APP_INITIAL_WIDTH;
int appHeight = APP_INITIAL_HEIGHT;

//Interface control
int lastMousePosX = 0;
int lastMousePosY = 0;
int mouseState = GLUT_UP;
int mouseButton = GLUT_RIGHT_BUTTON;

Frames fps;

typedef enum {Vertex, Pixel, Deferred} LightModel;
LightModel lightModel = Vertex;
bool enabledAntialias = false;

int antialiasNumTimes = 1;

bool polygonModeFill = true;

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
KernelAntiAliasing* kernelAntiAliasing;
KernelAntiAliasingN* kernelAntiAliasingN;

Shader * pixelShading;
GLFont fontRender;

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
  
  glClearColor(.8, .8, 1.0, 1.0);

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



void keyboardSpecial(int key, int x, int y){

	if( key == GLUT_KEY_LEFT ) camBeta = (int)(camBeta + camInc)%360;
	else if( key == GLUT_KEY_RIGHT )  camBeta = (int)(camBeta - camInc)%360;
	else if( key == GLUT_KEY_UP ) camAlpha = (int)(camAlpha - camInc)%360;
	else if( key == GLUT_KEY_DOWN )camAlpha = (int)(camAlpha + camInc)%360;

  switch(key)
  {
  case 10: //F10
    if(polygonModeFill)
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
       glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    polygonModeFill = !polygonModeFill;
    break;
  }

  
}

void keyboard(unsigned char key, int x, int y){
  switch(key)
  {
    case 27://ESC
      exit(42);
    break;
    case '1':
	    lightModel = Vertex;
      break;
    case '2':
      lightModel = Pixel;
      break;
    case '3':
      lightModel = Deferred;
      break;

    case 'A':
    case 'a':
      enabledAntialias = !enabledAntialias;
      if(enabledAntialias)
        cout << "AntiAliasing ON"<<endl;
      else cout << "AntiAliasing OFF"<<endl;
      break;
    case '+':
      antialiasNumTimes++;
      cout << "Antialising " <<antialiasNumTimes << "x"<<endl;
    break;
    case '-':
      antialiasNumTimes = max(antialiasNumTimes-1,1);
      cout << "Antialising " <<antialiasNumTimes << "x"<<endl;
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
//PointLight p2;
SpotLight sp2;
DirectionalLight d;


void createScenes()
{
  rtScene = new Scene("./resources/scenes/cavalo.rt4");
  //rtScene->configure();
  //rtScene->setLightEnabled(false);

  p.setAmbientColor(Color(0.0,0.0,0.0));
  p.setDiffuseColor(Color(.8,.8,.8));
  p.setSpecularColor(Color(1.,1.,1.));
  p.setPosition(Vector3(0,100,0));

  //p2.setAmbientColor(Color(0.0,0.0,0.0));
  //p2.setDiffuseColor(Color(.8,.8,.8));
  //p2.setSpecularColor(Color(1.,1.,1.));
  //p2.setPosition(Vector3(100,0,0));

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

  kernelAntiAliasing = new KernelAntiAliasing(appWidth, appHeight,
    kernelGeometry->getTexIdNormal());

  kernelAntiAliasingN = new KernelAntiAliasingN(appWidth, appHeight,
    kernelShade->getTexIdColor(),
    kernelAntiAliasing->getTexIdFactor());

  pixelShading = new Shader("./resources/Shaders/LightShader2.vert","./resources/Shaders/LightShader2.frag");
  
  
}



void render(){
  float fpsec = fps.getFrames();
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

 

  if(lightModel==Pixel)
  {
    pixelShading->setActive(true);
    GLuint loc = pixelShading->getUniformLocation("numLights");
    glUniform1i(loc, rtScene->getNumLights());
  }

  if(lightModel==Deferred)
  {
    kernelGeometry->setActive(true);
    glClearColor(.8, .8, 1.0, -1.0);
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    rtScene->setLightEnabled(false);
    glDisable(GL_LIGHTING);
  }else
  {
    //rtScene->setLightEnabled(true);
 /*   p2.configure();
	  p2.render();*/

    p.configure();
    p.render();

    d.configure();
    d.render();

    sp2.configure();
    sp2.render();
  }

  glPushAttrib(GL_ALL_ATTRIB_BITS);



  //glPushMatrix();

  //glScalef(10.,1.,10.);
  rtScene->configure();
  rtScene->render();

  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
  glColor3f(1,1,1);
  glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
  glColor3f(1,0,0);
  glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
  glColor3f(1,1,1);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10.0);
  ////
  ////glCullFace(GL_FRONT);
  ////for(int i=0;i<NL;++i)
  ////{
  ////  pp[i].configure();
  ////  pp[i].render();
  ////}



  //glutSolidTeapot(60);
  //glutSolidSphere(60, 30, 30);
  ////glutSolidSphere(60, 500, 500);
  //
  //glBegin(GL_QUADS);
  //  glNormal3f(0,0,1);
  //  glVertex3f(-301,-301,-300);
  //  glVertex3f(300,-301,-300);
  //  glVertex3f(300 ,300,-301);
  //  glVertex3f(-301,300,-301);
  //glEnd();

  //glPopMatrix();
  if(lightModel==Pixel)
    pixelShading->setActive(false);
  
  if(lightModel==Deferred)
  {
    kernelGeometry->setActive(false);
    //kernelGeometry->renderOutput(KernelGeometry::Diffuse);
    
    kernelShade->step(lightModelViewMatrix);
    if(!enabledAntialias)
      kernelShade->renderOutput(0);
    else 
    {
      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glLoadIdentity();
      gluOrtho2D(0, 1, 0, 1);
      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();
      glLoadIdentity();


      kernelAntiAliasing->step();
      //kernelAntiAliasing->renderOutput(0);
      for(int i=0;i<antialiasNumTimes; ++i)
        kernelAntiAliasingN->step();

      glMatrixMode(GL_PROJECTION);
      glPopMatrix();
      glMatrixMode(GL_MODELVIEW);
      glPopMatrix();
      kernelAntiAliasingN->renderOutput(0);
    }
    /**/
  }
    //kernelShade->renderShader(lightModelViewMatrix);




  fontRender.initText();

  fontRender.print(10,appHeight*.05, "(1) Vertex Shading", Color(0., 0., 0.));
  fontRender.print(10,appHeight*.05+25, "(2) Pixel Shading", Color(0., 0., 0.));
  fontRender.print(10,appHeight*.05+50, "(3) Deferred Shading", Color(0., 0., 0.));
  fontRender.print(10,appHeight*.05+75, "  (A) AntiAliasing", Color(0., 0., 0.));
  fontRender.print(10,appHeight*.05+100, "    (+) + AntiAliasing", Color(0., 0., 0.));
  fontRender.print(10,appHeight*.05+125,"    (-) - AntiAliasing", Color(0., 0., 0.));
  char a[100];

  sprintf(a,"%d K Triangles", rtScene->getSceneNumTriangles()/1000);
  fontRender.print(appWidth*.80,appHeight*.05,a, Color(0., 0., 0.));
  
  sprintf(a,"%d Lights", rtScene->getNumLights());
  fontRender.print(appWidth*.80,appHeight*.05 + 25,a, Color(0., 0., 0.));


  sprintf(a,"%.2f FPS", fpsec);
  fontRender.print(appWidth*.85,appHeight*.85+55,a, Color(0., 0., 0.));
  switch(lightModel)
  {
    case Vertex:
      fontRender.print(10,appHeight*.85+55,"Vertex Shading ON", Color(0., 0., 0.));
      break;
    case Pixel:
      fontRender.print(10,appHeight*.85+55,"Pixel Shading ON", Color(0., 0., 0.));
      break;
    case Deferred:
      fontRender.print(10,appHeight*.85+55,"Deferred Shading ON", Color(0., 0., 0.));
      if(enabledAntialias)
      {
        char alias[60];
        sprintf(alias, "AntiAliasing %dx ON", antialiasNumTimes);
        fontRender.print(10,appHeight*.85+85,alias, Color(0.,0.,0.));
      }
      else fontRender.print(10,appHeight*.85+85,"AntiAliasing OFF", Color(0., 0., 0.));
      break;
  }


  fontRender.endText();
  glPopAttrib();
}
