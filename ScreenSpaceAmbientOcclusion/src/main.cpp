#include <iostream>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <cmath>

#include "GraphBasis/Vector3.h"
#include "GraphBasis/Matrix4.h"
#include "GraphBasis/Shader.h"
#include "GraphBasis/GLFont.h"
#include "GraphBasis/Frames.h"

#include "GraphBasis/TextureObject.h"

#include "Light/PointLight.h"
#include "Light/SpotLight.h"
#include "Light/DirectionalLight.h"

#include "Objects/Scene.h"
#include "MeshLoaders/P3bMeshFile.h"
#include "Kernels/KernelDeferred.h"
#include "Kernels/KernelColor.h"
#include "Kernels/KernelDeferred_Peeling.h"
#include "Kernels/KernelSSAO.h"



#include "main.h"

//Screen Attributes
int appWidth = APP_INITIAL_WIDTH;
int appHeight = APP_INITIAL_HEIGHT;

//Camera Attributes
float nearPlane = APP_NEAR;
float farPlane = APP_FAR;
float fov = APP_FOV;

//Pipeline Config
bool polygonModeFill = true;

//Interface control
int lastMousePosX = 0;
int lastMousePosY = 0;
int mouseState = GLUT_UP;
int mouseButton = GLUT_RIGHT_BUTTON;

bool lights_on = true;
int render_model = 0;
int outputSelection = 0;
int numPeelings = 3;
int outputIndexSelection = 0;
bool shader_on = true;

//Camera Position
float camAlpha = 0.0;
float camBeta = 90.0;
float camR = 300.0;
float camInc = 5.0;

//Ligths
PointLight p;
//PointLight p2;
SpotLight sp2;
DirectionalLight d;

Frames fps;
float fpsec;

//Global Objects
Scene* rtScene;
KernelDeferred* kernelDeferred;
KernelDeferred_Peeling* kernelDeferred_Peeling;
KernelColor* kernelColor;
KernelSSAO* kernelSSAO;
P3bMeshFile* p3bMesh;


//Algorithm
float rfar = 10.0f;
float pixelmask_size = .8;

GLFont fontRender;

//Debug
GLenum e;
TextureObject *texDebug;


int main(int argc, char *argv[]){
	init(argc, argv);

  glutMainLoop();
}



void init(int argc, char *argv[]){
	glutInit(&argc, argv);
	glewInit();	

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
  
  glClearColor(.8, .8, 1.0, 3.0);

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
  int modifier = glutGetModifiers();

	if(key == GLUT_KEY_LEFT) camBeta = (int)(camBeta + camInc)%360;
	else if(key == GLUT_KEY_RIGHT)  camBeta = (int)(camBeta - camInc)%360;
	else if(key == GLUT_KEY_UP) camAlpha = (int)(camAlpha - camInc)%360;
	else if(key == GLUT_KEY_DOWN)camAlpha = (int)(camAlpha + camInc)%360;

  switch(key)
  {
    case 10: //F10
      if(polygonModeFill)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      else
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

      polygonModeFill = !polygonModeFill;
      break;
    case 11:
      shader_on = !shader_on;
      break;
    case GLUT_KEY_PAGE_UP:
      rfar = rfar + 1. * (modifier == GLUT_ACTIVE_SHIFT ? 1. : .1);
      break;
    case GLUT_KEY_PAGE_DOWN:
      rfar = max(rfar - 1. * (modifier == GLUT_ACTIVE_SHIFT ? 1. : .1) , 0.0);
      break;

    case GLUT_KEY_HOME:
      pixelmask_size = min(pixelmask_size + .01f * (modifier == GLUT_ACTIVE_SHIFT ? 10 : 1), .999f);
      break;
    case GLUT_KEY_END:
      pixelmask_size = max(pixelmask_size - .01f * (modifier == GLUT_ACTIVE_SHIFT ? 10 : 1) , 0.001f);
      break;
  }

  
}

void keyboard(unsigned char key, int x, int y){
  int modifier = glutGetModifiers();
  if(key > '0' && key < '9' + 1)
    outputIndexSelection = min(key - '0' - 1, numPeelings - 1);
  switch(key)
  {
    case 27://ESC
      exit(42);
    break;
    case '\'':
      outputSelection = !outputSelection;
      break;
    case '1':
      break;
    case '2':
      break;
    case '3':
      break;
    case 'M':
    case 'm':
      render_model = (render_model + 1) % 2;
      break;
    case 'L':
    case 'l':
      lights_on = !lights_on;
      break;
    case '*':
      camAlpha = 0.0;
      camBeta = 90.0;
      camR = 10.0;
      break;

    case '+':
      numPeelings++;
    break;
    case '-':
      numPeelings = max(numPeelings - 1, 1);
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
  int modifier = glutGetModifiers();
	if(mouseButton == GLUT_LEFT_BUTTON && mouseState == GLUT_DOWN){
		float angleX = (x - lastMousePosX)*.5;
		float angleY = (y - lastMousePosY)*.5;


		camAlpha = ((int)(camAlpha + angleY))%360;
		camBeta = ((int)(camBeta + angleX))%360;
	}
	else if(mouseButton == GLUT_RIGHT_BUTTON && mouseState == GLUT_DOWN){
		camR += (y - lastMousePosY)/(2.0*(modifier == GLUT_ACTIVE_SHIFT ? 10 : 1) );
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



void renderUIText()
{
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  fontRender.setSize(GLFont::Medium);
  fontRender.initText();

  //fontRender.print(10,appHeight*.05, "(1) Vertex Shading", Color(0., 0., 0.));
  //fontRender.print(10,appHeight*.05+25, "(2) Pixel Shading", Color(0., 0., 0.));
  //fontRender.print(10,appHeight*.05+50, "(3) Deferred Shading", Color(0., 0., 0.));
  //fontRender.print(10,appHeight*.05+75, "  (A) AntiAliasing", Color(0., 0., 0.));
  //fontRender.print(10,appHeight*.05+100, "    (+) + AntiAliasing", Color(0., 0., 0.));
  //fontRender.print(10,appHeight*.05+125,"    (-) - AntiAliasing", Color(0., 0., 0.));
  char a[100];
  int i = 0;
  //sprintf(a,"%d K Triangles", rtScene->getSceneNumTriangles()/1000);
  //fontRender.print(appWidth*.80,appHeight*.05 + 25*i++,a, Color(0., 0., 0.));

  //sprintf(a,"%d Lights", rtScene->getNumLights());
  //fontRender.print(appWidth*.80,appHeight*.05 + 25,a, Color(0., 0., 0.));
  float x = .70;
  float y = .03;
  sprintf(a,"(+/-)%d Num Peelings", numPeelings);
  fontRender.print(appWidth*x,appHeight*y + 25*i++,a, Color(0., 0., 0.));

  sprintf(a,"(PgUp/PgDn)rfar: %.2f ", rfar);
  fontRender.print(appWidth*x,appHeight*y + 25*i++,a, Color(0., 0., 0.));

  sprintf(a,"(Home/End)pixmask: %.3f ", pixelmask_size);
  fontRender.print(appWidth*x,appHeight*y + 25*i++,a, Color(0., 0., 0.));
  
  sprintf(a,"(F11)Shader %s", shader_on? "On":"Off");
  fontRender.print(appWidth*x,appHeight*y + 25*i++,a, Color(0., 0., 0.));

  sprintf(a,"(l)Lights %s", lights_on? "On":"Off");
  fontRender.print(appWidth*x,appHeight*y + 25*i++,a, Color(0., 0., 0.));

  sprintf(a,"(m)Model: %d", render_model);
  fontRender.print(appWidth*x,appHeight*y + 25*i++,a, Color(0., 0., 0.));



  sprintf(a,"%.2f FPS", fpsec);
  fontRender.print(appWidth*.85,appHeight*.85+55,a, Color(0., 0., 0.));


  fontRender.endText();
  glPopAttrib();
}

void setupCamera()
{
  float x = camR*sin(DEG_TO_RAD(camBeta))*cos(DEG_TO_RAD(camAlpha));
  float y = camR*sin(DEG_TO_RAD(camAlpha));
  float z = camR*cos(DEG_TO_RAD(camBeta))*cos(DEG_TO_RAD(camAlpha));

  float nextAlpha =  min(camAlpha + camInc,360.0f);

  float ux = sin(DEG_TO_RAD(camBeta))*cos(DEG_TO_RAD(nextAlpha)) - x;
  float uy = sin(DEG_TO_RAD(nextAlpha)) - y;
  float uz = cos(DEG_TO_RAD(camBeta))*cos(DEG_TO_RAD(nextAlpha)) - z;

  glLoadIdentity();
  gluLookAt(x,y,z, 0, 0, 0, ux, uy, uz);
  p.setPosition(Vector3(x,y,z));
}


void createScenes()
{
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

  GLfloat amb [] = {0.2f,0.2f,0.2f,1.0f};
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
  
  kernelDeferred = new KernelDeferred(appWidth, appHeight);



  




  GLfloat *pixels = new GLfloat[appWidth*appHeight*4];
  for(int i=0; i < appWidth*appHeight*4; ++i)
    pixels[i] = 0.0;
  
  GLuint dummyTexId;
  glGenTextures(1, &dummyTexId);
  glBindTexture(GL_TEXTURE_2D, dummyTexId);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); 
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, appWidth, appHeight, 0, GL_RGBA, GL_FLOAT, pixels);
  glBindTexture(GL_TEXTURE_2D, 0);


  kernelDeferred_Peeling = new KernelDeferred_Peeling(appWidth, appHeight
    ,dummyTexId ,3
  );

  kernelColor = new KernelColor(appWidth, appHeight);

  kernelSSAO = new KernelSSAO(appWidth, appHeight
    ,kernelDeferred_Peeling->getTexIdDiffuse(0)
    ,kernelColor->getTexIdColor()
    ,kernelDeferred_Peeling->getTexIdPosition(0)
    ,kernelDeferred_Peeling->getTexIdNormal(0)
    ,kernelDeferred_Peeling->getTexIdNormal(1)
    ,kernelDeferred_Peeling->getTexIdNormal(2)
  );


  rtScene = new Scene("./resources/scenes/cavalo.rt4");
  //rtScene->configure();
  //rtScene->setLightEnabled(false);



  texDebug = new TextureObject(kernelSSAO->getColorTexId());
    //texDebug = new TextureObject(kernelDeferred->getTexIdPosition());

  //p3bMesh = new P3bMeshFile();
  //p3bMesh->readFile("resources/Models/TecGraf/16metros_30graus.p3b");
  // int n = p3bMesh->getNumElements();
  //bool *b = new bool[n];
  //for(int i=0;i<n;++i)
  //  b[i] = rand()%2==0;
  //p3bMesh->setVisibleElements(b);
  //delete [] b;

  for(int i=0; i<rtScene->getNumMeshes();++i)
  {
    Mesh *m = rtScene->getMeshAt(i);
    P3bMeshFile * p3bMesh2 = m->getP3bMesh();
    if(p3bMesh2!=NULL)
    {
      int n = p3bMesh2->getNumElements();
      
      bool *b = new bool[n];
      for(int i=0;i<n;++i)
        b[i] = rand()%2==0;
      p3bMesh2->setVisibleElements(b);
      delete [] b;
    }
  }
}




void drawScene()
{
  glPushAttrib(GL_CURRENT_BIT|GL_LIGHTING_BIT);
  if(lights_on)
  {
    p.configure();
    p.render();
  }
  //glEnable(GL_CULL_FACE);
  rtScene->setLightEnabled(lights_on);
  rtScene->configure();
  rtScene->render();
  glPopAttrib();
}



void render(){
  fpsec = fps.getFrames();
  setupCamera();

  if(!shader_on)
  {
    drawScene();
  }else
  {
    kernelColor->setActive(true);

    glClearColor(.8, .8, 1.0, -1.0);
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    glDisable(GL_CULL_FACE);
    drawScene();

    kernelColor->setActive(false);




    GLfloat projectionMatrix[16];
    glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);

    for(int i=0; i < numPeelings; ++i)
    {
      kernelDeferred_Peeling->step(i);
      kernelDeferred_Peeling->setActive(true);

      glClearColor(.8, .8, 1.0, -1.0);
      glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
      glDisable(GL_CULL_FACE);
      drawScene();

      kernelDeferred_Peeling->setActive(false);
    }
    //kernelDeferred_Peeling->renderOutput(0);

    float x = projectionMatrix[0*4+0];
    float y = projectionMatrix[1*4+1];
    float z = projectionMatrix[2*4+2];
    float w = projectionMatrix[3*4+2];
    float Znear = w/(z - 1.0);
    float Zfar = w * Znear/(w + 2.0 * Znear);
    float right = Znear/x;
    float top = Znear/y;


    kernelSSAO->step(Znear, Zfar, right, top, rfar, pixelmask_size);
    kernelSSAO->renderOutput(0);


















/********************************
///VERIFICADOR DE PONTOS EM ESPAÇO DE TELA
/********************************

GLfloat *buffer = NULL;
float *radiuses = NULL;
Vector3 *positions = NULL; 
float mvi [16];
int numspheres = 0;
int t = 0;
    
    
    if(t == 0)
    {
      GLfloat lightModelViewMatrix[16];
      glGetFloatv(GL_MODELVIEW_MATRIX, lightModelViewMatrix);

      GLfloat modelViewMatrix[16];
      glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);

      Matrix4 mvi = Matrix4((float*)modelViewMatrix);
      mvi.Inverse();
      mvi.Transpose();
     
      GLfloat projectionMatrix[16];
      glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);

      for(int i=0; i < numPeelings; ++i)
      {
        kernelDeferred_Peeling->step(i);
        kernelDeferred_Peeling->setActive(true);

        glClearColor(.8, .8, 1.0, -1.0);
        glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
        glDisable(GL_CULL_FACE);
        //glEnable(GL_CULL_FACE);
        rtScene->configure();
        rtScene->render();

        kernelDeferred_Peeling->setActive(false);
      }
      //kernelDeferred_Peeling->renderOutput(0);

      float x = projectionMatrix[0*4+0];
      float y = projectionMatrix[1*4+1];
      float z = projectionMatrix[2*4+2];
      float w = projectionMatrix[3*4+2];
      float Znear = w/(z - 1.0);
      float Zfar = w * Znear/(w + 2.0 * Znear);
      float right = Znear/x;
      float top = Znear/y;
      

      kernelSSAO->step(Znear, Zfar, right, top);
      texDebug->setId(kernelSSAO->getColorTexId());
      
      
      
      buffer = texDebug->getTextureData();
      int width = texDebug->getTextureWidth();
      int height = texDebug->getTextureHeight();
      
      numspheres = 0;
      for(int i = height-1; i >= 0; --i)
      {
        for(int j = 0; j < width; ++j)
          if(buffer[i*width*4+j*4 + 3] < 0)
            ;
          else numspheres++;
      }

      if(radiuses)
        delete[] radiuses;
      if(positions)
        delete[] positions;

      radiuses = new float[numspheres];
      positions = new Vector3[numspheres];

      int k = 0;
      for(int i = height-1; i >= 0; --i)
      {
        for(int j = 0; j < width; ++j)
          if(buffer[i*width*4+j*4 + 3] < 0)
            ;
          else
          {
            positions[k] = mvi*Vector3(buffer[i*width*4+j*4 + 0], buffer[i*width*4+j*4 + 1], buffer[i*width*4+j*4 + 2]);
            radiuses[k] = buffer[i*width*4+j*4 + 3]; 
            k++;
          };
      }
      
      t++;
    }

    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

    glColor3f(1,1,1);

    rtScene->configure();
    rtScene->render();
    glColor3f(1,0,0);
    glBegin(GL_POINTS);
    for(int i=0; i<numspheres; ++i)
    {
      glPushMatrix();
      glTranslatef(positions[i].x,positions[i].y,positions[i].z);
      glutSolidCube(radiuses[i]);
      glPopMatrix();
      //glVertex3f(positions[i].x,positions[i].y,positions[i].z);
    }
    glEnd();

  ***************************************************************/
      
    



    /*if(outputSelection)
      texDebug->setId(kernelDeferred_Peeling->getTexIdNormal(outputIndexSelection));
    else 
      texDebug->setId(kernelDeferred_Peeling->getTexIdPosition(outputIndexSelection));
    texDebug->renderTexture();*/

    //GLfloat *buffer = texDebug->getTextureData();
    //int w = texDebug->getTextureWidth();
    //int h = texDebug->getTextureHeight();
    //printf("");

    //kernelDeferred_Peeling->renderOutput(1);
  /*
    kernelDeferred_Peeling->step(1);
    kernelDeferred_Peeling->setActive(true);

    glClearColor(.8, .8, 1.0, -1.0);
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    glDisable(GL_CULL_FACE);
    rtScene->configure();
    rtScene->render();

    kernelDeferred_Peeling->setActive(false);
    //kernelDeferred_Peeling->renderOutput(0);
    texDebug->setId(kernelDeferred_Peeling->getTexIdNormal(1));
    //texDebug->setId(kernelDeferred_Peeling->getTexIdPosition(1));
    texDebug->renderTexture();

    //kernelDeferred_Peeling->setActive(true);

    //glClearColor(.8, .8, 1.0, -1.0);
    //glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    //glDisable(GL_CULL_FACE);
    //rtScene->configure();
    //rtScene->render();

    //kernelDeferred_Peeling->setActive(false);
    //kernelDeferred_Peeling->renderOutput(KernelDeferred::Position);
  /*
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
      //p2.configure();
	    //p2.render();

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
    }
      //kernelShade->renderShader(lightModelViewMatrix);
  /**/
  }
  renderUIText();
 // glPopAttrib();
}
