#include <iostream>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <cmath>

#include "GraphBasis\Vector3.h"
#include "GraphBasis\Shader.h"
#include "GraphBasis/GLFont.h"
#include "GraphBasis/Frames.h"

#include "Kernels/KernelFresnel.h"

#include "Objects\Scene.h"

#include "main.h"


int appWidth = APP_INITIAL_WIDTH;
int appHeight = APP_INITIAL_HEIGHT;

//Interface control
int lastMousePosX = 0;
int lastMousePosY = 0;
int mouseState = GLUT_UP;
int mouseButton = GLUT_RIGHT_BUTTON;

Frames fps;

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
GLFont fontRender;

//Kernels
KernelFresnel *kernelFresnel;

//Program Options
typedef enum {Regular, SubSurfaceScattering} OptionsState;
OptionsState optionsState = Regular;

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
         glPolygonMode(GL_FRONT, GL_FILL);
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
      optionsState = Regular;
      cout << "Shader Off" << endl;
    break;
    case '2':
      optionsState = SubSurfaceScattering;
      cout << "Shader On" << endl;
    break;
    case '3':
    break;

    case 'A':
    case 'a':
    break;
    case '+':
    break;
    case '-':
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
  glutSwapBuffers();
}

void renderAxis(){
  glBegin(GL_LINES);
  glColor3f(0, 0, 1); glVertex3f(0, 0, 0); glColor3f(0, 0, 1); glVertex3f(0, 0, 1000);
  glColor3f(0, 1, 0); glVertex3f(0, 0, 0); glColor3f(0, 1, 0); glVertex3f(0, 1000, 0);
  glColor3f(1, 0, 0); glVertex3f(0, 0, 0); glColor3f(1, 0, 0); glVertex3f(1000, 0, 0);
  glEnd();
}



void renderScreenQuad(GLuint id = 0)
{
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0, 1, 0, 1);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  if(id != 0)
  {
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id);
  }
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
  if(id != 0)
  {
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
  }
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}



typedef struct
{
  int max_tex_size;
  int sizeofVertexInfo;
  int numVertices;
  int vertexInfoSize, vertexInfoW, vertexInfoH;
  int vertexNeighborSize, vertexNeighborIndexW, vertexNeighborIndexH;
  int vertexRSize, vertexNeighborRW, vertexNeighborRH;

  float *vertexInfo, *vertexNeighborIndex, *vertexNeighborR;

  GLuint vertexInfoTexId , vertexNeighborIndexTexId, vertexNeighborRTexId;
}StexFileInfo;
StexFileInfo stexFileInfo;

StexFileInfo createTexturesFromPreProcess(string fileName)
{
  StexFileInfo ret;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &ret.max_tex_size);

  FILE *fp;
  fp = fopen(fileName.c_str(), "rb");
  MyAssert("Invalid FileName: " + fileName, fp);

  fread(&ret.numVertices, sizeof(int), 1, fp);
  fread(&ret.sizeofVertexInfo, sizeof(int), 1, fp);
  fread(&ret.vertexInfoSize, sizeof(int), 1, fp);
  fread(&ret.vertexNeighborSize, sizeof(int), 1, fp);
  fread(&ret.vertexRSize, sizeof(int), 1, fp);

  ret.vertexInfoW = ret.vertexNeighborIndexW = ret.vertexNeighborRW = ret.max_tex_size;
  ret.vertexInfoH = (ret.vertexInfoSize/sizeof(float)/4)/ret.max_tex_size + 1;
  ret.vertexNeighborIndexH = (ret.vertexNeighborSize/sizeof(float)/4)/ret.max_tex_size + 1;
  ret.vertexNeighborRH = (ret.vertexRSize/sizeof(float)/4)/ret.max_tex_size + 1;

  ret.vertexInfo = new float[ret.vertexInfoW * ret.vertexInfoH * 4];
  fread(ret.vertexInfo, sizeof(float), ret.vertexInfoSize, fp );

  ret.vertexNeighborIndex = new float[ret.vertexNeighborIndexW * ret.vertexNeighborIndexH * 4];
  fread(ret.vertexNeighborIndex, sizeof(float), ret.vertexNeighborSize, fp );

  ret.vertexNeighborR = new float[ret.vertexNeighborRW * ret.vertexNeighborRH * 4];
  fread(ret.vertexNeighborR, sizeof(float), ret.vertexRSize, fp );

  fclose(fp);

  glGenTextures(1, &ret.vertexInfoTexId);
  glBindTexture(GL_TEXTURE_2D, ret.vertexInfoTexId);
  //   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  //   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap generation included in OpenGL v1.4
  //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, ret.vertexInfoW, ret.vertexInfoH, 0, GL_RGBA, GL_FLOAT, ret.vertexInfo);
  glBindTexture(GL_TEXTURE_2D, 0);


  glGenTextures(1, &ret.vertexNeighborIndexTexId);
  glBindTexture(GL_TEXTURE_2D, ret.vertexNeighborIndexTexId);
  //   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  //   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap generation included in OpenGL v1.4
  //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, ret.vertexNeighborIndexW, ret.vertexNeighborIndexH, 0, GL_RGBA, GL_FLOAT, ret.vertexNeighborIndex);
  glBindTexture(GL_TEXTURE_2D, 0);


  glGenTextures(1, &ret.vertexNeighborRTexId);
  glBindTexture(GL_TEXTURE_2D, ret.vertexNeighborRTexId);
  //   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  //   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap generation included in OpenGL v1.4
  //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, ret.vertexNeighborRW, ret.vertexNeighborRH, 0, GL_RGBA, GL_FLOAT, ret.vertexNeighborR);
  glBindTexture(GL_TEXTURE_2D, 0);

  return ret;
}

void createScenes()
{
  rtScene = new Scene("./resources/scenes/scene1.rt4");
  //rtScene->configure();
  //rtScene->setLightEnabled(false);

  GLfloat amb [] = {0.2,0.2,0.2,1};
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
  string path = "./resources/Models/";
  string fileName = "dragon_low";

  stexFileInfo = createTexturesFromPreProcess(path+fileName+".stex");

  kernelFresnel = new KernelFresnel(stexFileInfo.vertexInfoW, stexFileInfo.vertexInfoH, stexFileInfo.vertexInfoTexId
                                      ,stexFileInfo.numVertices, stexFileInfo.sizeofVertexInfo);

/*    
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
  */
  
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
  
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  //glPushMatrix();

  switch(optionsState)
  {
    case Regular:
       renderScreenQuad(kernelFresnel->getTexIdFresnel());
    break;
    case SubSurfaceScattering:

      kernelFresnel->setActive(true);
      /*glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);*/
      glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
      
      glViewport(0, 0,  stexFileInfo.vertexInfoW, stexFileInfo.vertexInfoH); //Render the texture as full screen
      renderScreenQuad();
    break;
  }

  
 /* glBegin(GL_POINTS);
    glColor3f(1, 0, 0);
    glTexCoord1d(0);
    glVertex3f(0, 0, -10); 
    glColor3f(0, 1, 0);
    glTexCoord1d(1);
    glVertex3f(100, 0, -10);
    glColor3f(0, 0, 1);
    glTexCoord1d(2);
    glVertex3f(100, 100, -10);
    glColor3f(1, 0, 1);
    glTexCoord1d(3);
    glVertex3f(0, 100, -10);
  glEnd();
  
  rtScene->configure();
  rtScene->render();*/

  //glutSolidTeapot(60);

  switch(optionsState)
  {
    case Regular:
    break;
    case SubSurfaceScattering:
      kernelFresnel->setActive(false);
      kernelFresnel->renderOutput(KernelFresnel::Fresnel);
    break;
   }



  //glEnable(GL_COLOR_MATERIAL);
  //glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
  //glColor3f(1,1,1);
  //glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
  //glColor3f(1,0,0);
  //glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
  //glColor3f(1,1,1);
  //glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10.0);

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
  
  //if(lightModel==Pixel)
  //  pixelShading->setActive(false);
  //
  //if(lightModel==Deferred)
  //{
  //  kernelGeometry->setActive(false);
  //  //kernelGeometry->renderOutput(KernelGeometry::Diffuse);
  //  
  //  kernelShade->step(lightModelViewMatrix);
  //  if(!enabledAntialias)
  //    kernelShade->renderOutput(0);
  //  else 
  //  {
  //    glMatrixMode(GL_PROJECTION);
  //    glPushMatrix();
  //    glLoadIdentity();
  //    gluOrtho2D(0, 1, 0, 1);
  //    glMatrixMode(GL_MODELVIEW);
  //    glPushMatrix();
  //    glLoadIdentity();


  //    kernelAntiAliasing->step();
  //    //kernelAntiAliasing->renderOutput(0);
  //    for(int i=0;i<antialiasNumTimes; ++i)
  //      kernelAntiAliasingN->step();

  //    glMatrixMode(GL_PROJECTION);
  //    glPopMatrix();
  //    glMatrixMode(GL_MODELVIEW);
  //    glPopMatrix();
  //    kernelAntiAliasingN->renderOutput(0);
  //  }
  //}


  fontRender.initText();

  char a[100];

  sprintf(a,"%d K Triangles", rtScene->getSceneNumTriangles()/1000);
  fontRender.print(appWidth*.80,appHeight*.05,a, Color(0., 0., 0.));
  
  sprintf(a,"%d Lights", rtScene->getNumLights());
  fontRender.print(appWidth*.80,appHeight*.05 + 25,a, Color(0., 0., 0.));

  sprintf(a,"%.2f FPS", fpsec);
  fontRender.print(appWidth*.85,appHeight*.85+55,a, Color(0., 0., 0.));

  fontRender.endText();
  glPopAttrib();
}
