#include <iostream>
#include <GL\glew.h>
#include <GL\glut.h>
#include <cmath>

#include "MathUtils/Vector3.h"
#include "MathUtils/Matrix4.h"
#include "GLUtils/GLFont.h"
#include "Objects/Frames.h"

#include "GLUtils/GLTextureObject.h"

#include "GLLights/GLPointLight.h"

#include "ScScene/ScScene.h"
#include "MeshLoaders/P3bMeshFile.h"
#include "Kernels/KernelColor.h"
#include "Kernels/KernelDeferred_Peeling.h"
#include "Kernels/KernelSSAO.h"
#include "Kernels/KernelBlurr.h"
#include "Kernels/KernelCombine.h"

#include "Handlers/SphereGLCameraHandler.h"

#include "main.h"

//Application Parameters
int appWidth = APP_INITIAL_WIDTH;
int appHeight = APP_INITIAL_HEIGHT;
float nearPlane = APP_NEAR;
float farPlane = APP_FAR;
float fov = APP_FOV;
GLfloat clearColor[] = {.8, .8, 1.0, -1.};


//Render Objects
SphereGLCameraHandler *camHandler;
GLFont fontRender;
Frames fps;
float fpsec;

//Scene Objects
Scene* rtScene;
P3bMeshFile* p3bMesh;

//Kernel Objects
KernelDeferred_Peeling* kernelDeferred_Peeling;
KernelColor* kernelColor;
KernelSSAO* kernelSSAO;
KernelBlur* kernelBlurr;
KernelCombine* kernelCombine;

//Interface control
bool menu_on = true;
bool lights_on = false;
bool minerLight_on = false;
bool wireframe_on = false;
bool shader_on = true;
bool shader_active = !wireframe_on & shader_on;

//Algorithm Parameters
float rfar = 30.0f;
float pixelmask_size = .8;
float offsets_size = 5.0;
float intensity = 20.0;
int numPeelings = 3;
bool blurr_on = false;

//Debug
GLenum e;
GLTextureObject *texDebug;



int main(int argc, char *argv[]){
	initGL(argc, argv);

  loadScene();

  createKernels();

  reshapeGL(appWidth, appHeight);
  glutMainLoop();
}



void initGL(int argc, char *argv[]){
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
	glutReshapeFunc(reshapeGL);
  
  glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);

  glShadeModel(GL_SMOOTH);
  // FILLED 
  glPolygonMode(GL_FRONT, GL_FILL);
  // WIREFRAME Clean
  // glPolygonMode(GL_BACK, GL_LINE); 

  // WIREFRAME Dense
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 

  glCullFace(GL_BACK); 
  //Enable Culling
  //glEnable(GL_CULL_FACE);

  glEnable(GL_DEPTH_TEST);
  
  //Disables cursor
  //glutSetCursor(GLUT_CURSOR_NONE);
}


void reshapeGL(int w, int h){
	glViewport (0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective(fov, (GLfloat)w / (GLfloat)h, nearPlane, farPlane);
  glMatrixMode (GL_MODELVIEW);

  appWidth = w;
  appHeight = h;
}



void keyboardSpecial(int key, int x, int y){
  camHandler->listenKeyboard(key);

  int modifier = glutGetModifiers();

  switch(key)
  {
    case 1://F1
      menu_on = !menu_on;
      break;

    case 5: //F5
      rfar = 30.0f;
      pixelmask_size = .8;
      offsets_size = 5.0;
      intensity = 20.0;
      break;

    case 10: //F10
      wireframe_on = !wireframe_on;
      shader_active = !wireframe_on & shader_on;

      if(wireframe_on)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      else
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      break;

    case 11:
      shader_on = !shader_on;
      shader_active = wireframe_on & shader_on;
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

  switch(key)
  {
    case 27://ESC
      exit(42);
    break;

    case 'M':
    case 'm':
      minerLight_on = !minerLight_on;
      break;

    case 'L':
    case 'l':
      lights_on = !lights_on;
      break;

    case 'B':
    case 'b':
      blurr_on = !blurr_on;
      break;

    case '+':
      numPeelings++;
    break;
    case '-':
      numPeelings = max(numPeelings - 1, 1);
    break;

    case 'K':
      offsets_size = min(offsets_size + 1.0f, appWidth/2.0f);
      break;
    case 'k':
      offsets_size = min(offsets_size + .1f, appWidth/2.0f);
      break;
    case 'J':
      offsets_size = max(offsets_size - 1.0f, 1.0f);
      break;
    case 'j':
      offsets_size = max(offsets_size - .1f, 1.0f);
      break;

    case 'R':
    case 'r':
      kernelDeferred_Peeling->reloadShader();
      kernelColor->reloadShader();
      kernelSSAO->reloadShader();
      kernelBlurr->reloadShader();
      kernelCombine->reloadShader();
      break;

    case 'I':
      intensity = intensity + (intensity > 1000.0f? 100.0f :(intensity > 100.0f? 10.0f : 1.0f));
      break;
    case 'i':
      intensity = intensity + .05f;
      break;

    case 'U':
      intensity = max(intensity -(intensity > 100.0f? 10.0f : 1.0f), 0.01f);
      break;
    case 'u':
      intensity = max(intensity - .05f, 0.01f);
      break;
  }
}

void mouseButtons(int button, int state, int x, int y){
  camHandler->listenMouseClick(button, state, x, y);
}

void mouseActive(int x, int y){
  camHandler->listenMouseMove(x, y);
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  render();
  glutSwapBuffers();
}

void renderGUI()
{
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  fontRender.setSize(GLFont::Medium);
  fontRender.initText(appWidth, appHeight);

  char a[100];
  int i = 0;
  //sprintf(a,"%d K Triangles", rtScene->getSceneNumTriangles()/1000);
  //fontRender.print(appWidth*.80,appHeight*.05 + 25*i++,a, Color(0., 0., 0.));

  //sprintf(a,"%d Lights", rtScene->getNumLights());
  //fontRender.print(appWidth*.80,appHeight*.05 + 25,a, Color(0., 0., 0.));
  float x = .70;
  float y = .03;

  sprintf(a,"(F1)%s Menu", menu_on?"Close":"Open");
  fontRender.print(appWidth*x,appHeight*y + 25*i++,a, Color(0., 0., 0.));

  if(menu_on)
  {
    sprintf(a,"(+/-)%d Num Peelings", numPeelings);
    fontRender.print(appWidth*x,appHeight*y + 25*i++,a, Color(0., 0., 0.));

    sprintf(a,"(PgUp/PgDn)rfar: %.2f ", rfar);
    fontRender.print(appWidth*x,appHeight*y + 25*i++,a, Color(0., 0., 0.));

    sprintf(a,"(Home/End)pixmask: %.3f ", pixelmask_size);
    fontRender.print(appWidth*x,appHeight*y + 25*i++,a, Color(0., 0., 0.));

    sprintf(a,"(K/J)offsets_size: %.1f ", offsets_size);
    fontRender.print(appWidth*x,appHeight*y + 25*i++,a, Color(0., 0., 0.));
    
    sprintf(a,"(I/U)intensity: %.2f ", intensity);
    fontRender.print(appWidth*x,appHeight*y + 25*i++,a, Color(0., 0., 0.));
    
    sprintf(a,"(F11)Shader %s", shader_active? "On":"Off");
    fontRender.print(appWidth*x,appHeight*y + 25*i++,a, Color(0., 0., 0.));

    sprintf(a,"(b)AO Blurr %s", blurr_on? "On":"Off");
    fontRender.print(appWidth*x,appHeight*y + 25*i++,a, Color(0., 0., 0.));

    sprintf(a,"(l)Lights %s", lights_on? "On":"Off");
    fontRender.print(appWidth*x,appHeight*y + 25*i++,a, Color(0., 0., 0.));

    sprintf(a,"(m)Mine Light: %s", minerLight_on? "On":"Off");
    fontRender.print(appWidth*x,appHeight*y + 25*i++,a, Color(0., 0., 0.));
  }


  sprintf(a,"%.2f FPS", fpsec);
  fontRender.print(appWidth*.85,appHeight*.85+55,a, Color(0., 0., 0.));


  fontRender.endText();
  glPopAttrib();
}



void createKernels() 
{
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
    ,kernelDeferred_Peeling->getTexIdPosition(0)
    ,kernelDeferred_Peeling->getTexIdNormal(0)
    ,kernelDeferred_Peeling->getTexIdNormal(1)
    ,kernelDeferred_Peeling->getTexIdNormal(2)
    );

  kernelBlurr = new KernelBlur(appWidth, appHeight, kernelSSAO->getColorTexId());
  kernelCombine = new KernelCombine(appWidth, appHeight, kernelColor->getTexIdColor());

  texDebug = new GLTextureObject(kernelSSAO->getColorTexId());
}

void loadScene()
{
  camHandler = new SphereGLCameraHandler(10.f, 0.f, 90.f, 5.f);
  GLLight *minerLight = camHandler->getMinerLight();
  minerLight->setAmbientColor(Color(0.0,0.0,0.0));
  minerLight->setDiffuseColor(Color(.8,.8,.8));
  minerLight->setSpecularColor(Color(1.,1.,1.));
  minerLight->setPosition(Vector3(0,100,0));

  rtScene = new Scene("./resources/scenes/cavalo.rt4");

  for(int i=0; i<rtScene->getNumMeshes();++i)
  {
    ScMesh *m = rtScene->getMeshAt(i);
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

  if(rtScene->getNumMeshes() == 0)
    cout << "No Mesh Loaded!!" <<endl;
  
  GLfloat amb [4] = {0,0,0,1};
  Color ambientColor = rtScene->getAmbientColor();
  amb[0] = ambientColor.r;
  amb[1] = ambientColor.g;
  amb[2] = ambientColor.b;
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);

  Color sceneClearColor = rtScene->getClearColor();
  clearColor[0] = sceneClearColor.r;
  clearColor[1] = sceneClearColor.g;
  clearColor[2] = sceneClearColor.b;

  nearPlane = rtScene->getCamera()->getNear();
  farPlane = rtScene->getCamera()->getFar();
  fov = rtScene->getCamera()->getFovy();
  appWidth = rtScene->getCamera()->getScreenWidth();
  appHeight = rtScene->getCamera()->getScreenHeight();
}




void drawScene()
{
  glPushAttrib(GL_CURRENT_BIT|GL_LIGHTING_BIT);
    camHandler->setMinerLightOn(minerLight_on);
    camHandler->renderMinerLight();
    rtScene->setSceneLightEnabled(lights_on);
    rtScene->configure();
    rtScene->render();
  glPopAttrib();
}


GLfloat *buffer = NULL;
float *radiuses = NULL;
Vector3 *positions = NULL; 
float mvi [16];
int numspheres = 0;
int t = 0;
void testScreenShpereConverter();
bool test_sphere = false;

void render(){
  fpsec = fps.getFrames();
  camHandler->setMinerLightOn(false);
  camHandler->render();

  if(!shader_active)
  {
    drawScene();
    t = 0;
  }else if(test_sphere){
    testScreenShpereConverter();
  }else
  {
//COLOR PASS
//COLOR PASS
//COLOR PASS
    kernelColor->setActive(true);

    glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    glDisable(GL_CULL_FACE);
    drawScene();

    kernelColor->setActive(false);



    GLfloat modelviewMatrix[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, modelviewMatrix);
    GLfloat projectionMatrix[16];
    glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);

    Matrix4 mv = Matrix4((float*)modelviewMatrix);
    Matrix4 mp = Matrix4((float*)projectionMatrix);
    Matrix4 mvp = mp;
    mvp.Inverse();
    //mvp.Transpose();
    GLfloat imvp[16];
    for(int i=0;i<4;++i)
      for(int j = 0;j<4;++j)
        imvp[i*4 + j] = mvp.getValue(i, j);

//DEPTH PEELING PASS
//DEPTH PEELING PASS
//DEPTH PEELING PASS
    for(int i=0; i < numPeelings; ++i)
    {
      kernelDeferred_Peeling->step(i);
      kernelDeferred_Peeling->setActive(true);

      glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
      glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
      glDisable(GL_CULL_FACE);
      drawScene();

      kernelDeferred_Peeling->setActive(false);
    }
    //kernelDeferred_Peeling->renderOutput(0);


//SSAO PASS
//SSAO PASS
//SSAO PASS
    float x = projectionMatrix[0*4+0];
    float y = projectionMatrix[1*4+1];
    float z = projectionMatrix[2*4+2];
    float w = projectionMatrix[3*4+2];
    float Znear = w/(z - 1.0);
    float Zfar = w * Znear/(w + 2.0 * Znear);
    float right = Znear/x;
    float top = Znear/y;

    kernelSSAO->step(projectionMatrix, rfar, pixelmask_size,offsets_size, intensity);

//BLURR PASS
//BLURR PASS
//BLURR PASS
    if(blurr_on)
      kernelBlurr->step(1);

//COMBINE PASS
//COMBINE PASS
//COMBINE PASS
    if(blurr_on)
      kernelCombine->step(kernelBlurr->getBlurredTexId());
    else kernelCombine->step(kernelSSAO->getColorTexId());

//RENDER RESULT
//RENDER RESULT
//RENDER RESULT
  kernelCombine->renderOutput(0);
  }
  renderGUI();
}

void testScreenShpereConverter()
{
/********************************
///VERIFICADOR DE PONTOS EM ESPAÇO DE TELA
/********************************/

  if(t == 0)
  {
    GLfloat modelViewMatrix[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);

    Matrix4 mvi = Matrix4((float*)modelViewMatrix);
    mvi.Inverse();
    mvi.Transpose();
   



    //GLfloat modelviewMatrix[16];
    //glGetFloatv(GL_MODELVIEW_MATRIX, modelviewMatrix);
    GLfloat projectionMatrix[16];
    glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);

    //Matrix4 mv = Matrix4((float*)modelviewMatrix);
    //Matrix4 mp = Matrix4((float*)projectionMatrix);
    //Matrix4 mvp = mp;
    //mvp.Inverse();
    ////mvp.Transpose();
    //GLfloat imvp[16];
    //for(int i=0;i<4;++i)
    //  for(int j = 0;j<4;++j)
    //    imvp[i*4 + j] = mvp.getValue(i, j);

    //DEPTH PEELING PASS
    //DEPTH PEELING PASS
    //DEPTH PEELING PASS
    for(int i=0; i < numPeelings; ++i)
    {
      kernelDeferred_Peeling->step(i);
      kernelDeferred_Peeling->setActive(true);

      glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
      glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
      glDisable(GL_CULL_FACE);
      drawScene();

      kernelDeferred_Peeling->setActive(false);
    }
    //kernelDeferred_Peeling->renderOutput(0);


    //SSAO PASS
    //SSAO PASS
    //SSAO PASS
    float x = projectionMatrix[0*4+0];
    float y = projectionMatrix[1*4+1];
    float z = projectionMatrix[2*4+2];
    float w = projectionMatrix[3*4+2];
    float Znear = w/(z - 1.0);
    float Zfar = w * Znear/(w + 2.0 * Znear);
    float right = Znear/x;
    float top = Znear/y;

    kernelSSAO->step(projectionMatrix, rfar, pixelmask_size,offsets_size, intensity);
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
          Vector3 v1 = Vector3(buffer[i*width*4+j*4 + 0], buffer[i*width*4+j*4 + 1], buffer[i*width*4+j*4 + 2]);
          Vector3 v2 = v1 + Vector3(0,1,0)*buffer[i*width*4+j*4 + 3];
          Vector3 v3 = v2 - v1;
          v3 = mvi*v3;
          float ll =  buffer[i*width*4+j*4 + 3]; 
          float l = ~v3;
          radiuses[k] = ll; 

          k++;
        }
    }
    
    t++;
  }

  glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

  glColor3f(1,1,1);

  //rtScene->configure();
  //rtScene->render();
  glColor3f(1,0,0);
  //glBegin(GL_POINTS);
  
  for(int i=0; i<numspheres; i+=201)
      //for(int i=numspheres*.4; i<(numspheres*.4+numspheres/100); ++i)
  {
    glPushMatrix();
    glTranslatef(positions[i].x,positions[i].y,positions[i].z);
    glutSolidCube(radiuses[i]);
    glPopMatrix();
    printf("%d - %f %f %f - %f\n",i,positions[i].x, positions[i].y, positions[i].z, radiuses[i]);
    //glVertex3f(positions[i].x,positions[i].y,positions[i].z);
  }
  //int i = 32000;
  //glPushMatrix();
  //glTranslatef(positions[i].x/100,positions[i].y/100,positions[i].z/100);
  //glutSolidCube(radiuses[i]/10);
  //glPopMatrix();
  //printf("%d %f\n",i, radiuses[i]);
  //i = 0;
  //glPushMatrix();
  //glTranslatef(positions[i].x,positions[i].y,positions[i].z);
  //glutSolidCube(radiuses[i]*2);
  //glPopMatrix();
  //printf("%d %f\n",i, radiuses[i]);
  printf("\n\n");
  //glEnd();
  //t = 0;

  /****************************************************************/
}