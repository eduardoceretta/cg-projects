/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  May 2011
 *
 *  Receive a Scene description file renders the scene with ambient occlusion.
 *
 *  Initializes the scene, process the geometry via different kernel
 *  passes and combine results to produce the final scene with the
 *  ambient occlusion.
 */
#include <iostream>
#include <GL\glew.h>
#include <GL\glut.h>
#include <cmath>

#include "MathUtils/Vector3.h"
#include "MathUtils/Matrix4.h"

#include "GLUtils/GLFont.h"
#include "GLLights/GLPointLight.h"

#include "Objects/Frames.h"

#include "Handlers/SphereGLCameraHandler.h"

#include "ScScene/ScScene.h"
#include "MeshLoaders/P3bMeshFile.h"

#include "Kernels/KernelColor.h"
#include "Kernels/KernelDeferred_Peeling.h"
#include "Kernels/KernelSSAO.h"
#include "Kernels/KernelBlur.h"
#include "Kernels/KernelCombine.h"

#include "app.h"

/**
 * Tests
 */
#include "tests.h"

#ifdef TIME_TEST
TimeTest timeTest;
#endif

#ifdef SCREENSHOT_TEST
ScreenShotTest screenShotTest;
#endif


/*******************************************/
/* App                                     */
/*******************************************/
App::App()
:m_argc(0)
,m_argv(NULL)
,m_scenePath(APP_DEFAULT_SCENE_PATH)
,m_shaderPath(APP_DEFAULT_SHADER_PATH)
,m_appWidth(APP_INITIAL_WIDTH)
,m_appHeight(APP_INITIAL_HEIGHT)
,m_nearPlane(APP_NEAR)
,m_farPlane(APP_FAR)
,m_fov(APP_FOV)
,m_camHandler(NULL)
,m_fontRender(new GLFont())
,m_frames(new Frames())
,m_fps(0.0f)
,m_rtScene(NULL)
,m_p3bMesh(NULL)
,m_kernelDeferred_Peeling(NULL)
,m_kernelColor(NULL)
,m_kernelSSAO(NULL)
,m_kernelBlur(NULL)
,m_kernelCombine(NULL)
,m_menu_on(true)
,m_lights_on(false)
,m_minerLight_on(false)
,m_wireframe_on(false)
,m_shader_on(true)
,m_shader_active(!m_wireframe_on & m_shader_on)
,m_rfar(30.0f)
,m_pixelmaskSize(.8f)
,m_offsetSize(5.0f)
,m_intensity(20.0f)
,m_numPeelings(3)
,m_blurr_on(false)
{
  m_clearColor[0] = .8f;
  m_clearColor[1] = .8f;
  m_clearColor[2] = 1.0f;
  m_clearColor[3] = -1.;

  //Initializes Arguments
  m_acceptedArgsString["-scenepath"] = &m_scenePath;
  m_acceptedArgsString["-shaderpath"] = &m_shaderPath;

  m_acceptedArgsFloat["-rfar"] = &m_rfar;
  m_acceptedArgsFloat["-offset"] = &m_offsetSize;
  m_acceptedArgsFloat["-intensity"] = &m_intensity;
}

App::~App()
{
  delete m_fontRender;
  delete m_frames;

  if(m_camHandler)
    delete m_camHandler;

  if(m_rtScene)
    delete m_rtScene;

  if(m_p3bMesh)
    delete m_p3bMesh;

  if(m_kernelDeferred_Peeling)
    delete m_kernelDeferred_Peeling;

  if(m_kernelColor)
    delete m_kernelColor;

  if(m_kernelSSAO)
    delete m_kernelSSAO;

  if(m_kernelBlur)
    delete m_kernelBlur;

  if(m_kernelCombine)
    delete m_kernelCombine;


#ifdef TIME_TEST
  timeTest.logResults();
#endif
}


void App::initGL(int *argc, char *argv[])
{
  glutInit(argc, argv);
  glewInit();	

  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(m_appWidth, m_appHeight);
  glutInitWindowPosition(100, 100);
  glutCreateWindow(APP_NAME);

  glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);

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

  m_argc = *argc;
  m_argv = argv;
}

void App::loadResources()
{
  loadArgs();
  loadScene();
  loadKernels();
}

void App::render()
{
#ifdef TIME_TEST
  timeTest.update();
  timeTest.totalTimer.reset();
#endif

#ifdef SCREENSHOT_TEST
  screenShotTest.update();
  screenShotTest.configureCamera(m_camHandler);
#endif

  m_frames->update();
  m_camHandler->setMinerLightOn(false);
  m_camHandler->render();

  if(!m_shader_active)
  {
    drawScene();
  }else
  {
#ifdef TIME_TEST
    timeTest.resetTimer();
#endif
    //COLOR PASS
    m_kernelColor->setActive(true);

    glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    glDisable(GL_CULL_FACE);
    drawScene();

    m_kernelColor->setActive(false);
#ifdef TIME_TEST
  timeTest.kColorTime += timeTest.getTime();    
#endif
    

    GLfloat projectionMatrix[16];
    glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);


#ifdef TIME_TEST
    timeTest.resetTimer();
#endif
    //DEPTH PEELING PASS
    for(int i=0; i < m_numPeelings; ++i)
    {
      m_kernelDeferred_Peeling->step(i);
      m_kernelDeferred_Peeling->setActive(true);

      glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
      glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
      glDisable(GL_CULL_FACE);
      drawScene();

      m_kernelDeferred_Peeling->setActive(false);
    }
#ifdef TIME_TEST
    timeTest.kDeferedPeelingTime += timeTest.getTime();
#endif

#ifdef TIME_TEST
    timeTest.resetTimer();
#endif
    //SSAO PASS
    m_kernelSSAO->step(projectionMatrix, m_rfar, m_pixelmaskSize,m_offsetSize, m_intensity);
#ifdef TIME_TEST
    timeTest.kSSAOTime += timeTest.getTime();
#endif

#ifdef TIME_TEST
    timeTest.resetTimer();
#endif
    //BLURR PASS
    if(m_blurr_on)
      m_kernelBlur->step(1);
#ifdef TIME_TEST
    timeTest.kBlurTime += timeTest.getTime();
#endif

#ifdef TIME_TEST
    timeTest.resetTimer();
#endif
    //COMBINE PASS
    if(m_blurr_on)
      m_kernelCombine->step(m_kernelBlur->getBlurredTexId());
    else m_kernelCombine->step(m_kernelSSAO->getColorTexId());
#ifdef TIME_TEST
    timeTest.kCombineTime += timeTest.getTime();
#endif


    //RENDER RESULT
    m_kernelCombine->renderOutput(0);
  }
  renderGUI();

#ifdef TIME_TEST
  timeTest.totalTime += timeTest.totalTimer.getTime();
  //timeTest.printPartialResults();
#endif

#ifdef SCREENSHOT_TEST
  if(screenShotTest.isTestEnded())
    exit(42);
  screenShotTest.save(m_kernelCombine->getColorTexId());
#endif
}

void App::listenReshape( int w, int h )
{
  //Do not resize 
  //appWidth = w;
  //appHeight = h;
  glViewport (0, 0, (GLsizei)m_appWidth, (GLsizei)m_appHeight);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective(m_fov, (GLfloat)m_appWidth / (GLfloat)m_appHeight, m_nearPlane, m_farPlane);
  glMatrixMode (GL_MODELVIEW);
}

void App::listenKeyboard( int key )
{
  int modifier = glutGetModifiers();

  switch(key)
  {
  case 27://ESC
    exit(42);
    break;

  case 'M':
  case 'm':
    m_minerLight_on = !m_minerLight_on;
    break;

  case 'L':
  case 'l':
    m_lights_on = !m_lights_on;
    break;

  case 'B':
  case 'b':
    m_blurr_on = !m_blurr_on;
    break;

  case '=':
  case '+':
    m_numPeelings++;
    break;
  case '_':
  case '-':
    m_numPeelings = max(m_numPeelings - 1, 1);
    break;

  case 'K':
    m_offsetSize = min(m_offsetSize + 1.0f, m_appWidth/2.0f);
    break;
  case 'k':
    m_offsetSize = min(m_offsetSize + .1f, m_appWidth/2.0f);
    break;
  case 'J':
    m_offsetSize = max(m_offsetSize - 1.0f, 1.0f);
    break;
  case 'j':
    m_offsetSize = max(m_offsetSize - .1f, 1.0f);
    break;

  case 'R':
  case 'r':
    m_kernelDeferred_Peeling->reloadShader();
    m_kernelColor->reloadShader();
    m_kernelSSAO->reloadShader();
    m_kernelBlur->reloadShader();
    m_kernelCombine->reloadShader();
    break;

  case 'I':
    m_intensity = m_intensity + (m_intensity > 1000.0f? 100.0f :(m_intensity > 100.0f? 10.0f : 1.0f));
    break;
  case 'i':
    m_intensity = m_intensity + .05f;
    break;

  case 'U':
    m_intensity = max(m_intensity -(m_intensity > 100.0f? 10.0f : 1.0f), 0.01f);
    break;
  case 'u':
    m_intensity = max(m_intensity - .05f, 0.01f);
    break;
  }
}

void App::listenKeyboardSpecial( int key )
{
  m_camHandler->listenKeyboard(key);

  int modifier = glutGetModifiers();

  switch(key)
  {
  case 1://F1
    m_menu_on = !m_menu_on;
    break;

  case 5: //F5
    m_rfar = 30.0f;
    m_pixelmaskSize = .8;
    m_offsetSize = 5.0;
    m_intensity = 20.0;
    break;

  case 10: //F10
    m_wireframe_on = !m_wireframe_on;
    m_shader_active = !m_wireframe_on & m_shader_on;

    if(m_wireframe_on)
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    break;

  case 11:
    m_shader_on = !m_shader_on;
    m_shader_active = !m_wireframe_on & m_shader_on;
    break;

  case GLUT_KEY_PAGE_UP:
    m_rfar = m_rfar + 1. * (modifier == GLUT_ACTIVE_SHIFT ? 1. : .1);
    break;

  case GLUT_KEY_PAGE_DOWN:
    m_rfar = max(m_rfar - 1. * (modifier == GLUT_ACTIVE_SHIFT ? 1. : .1) , 0.0);
    break;

  case GLUT_KEY_HOME:
    m_pixelmaskSize = min(m_pixelmaskSize + .01f * (modifier == GLUT_ACTIVE_SHIFT ? 10 : 1), .999f);
    break;

  case GLUT_KEY_END:
    m_pixelmaskSize = max(m_pixelmaskSize - .01f * (modifier == GLUT_ACTIVE_SHIFT ? 10 : 1) , 0.001f);
    break;
  }
}

void App::listenMouseMove( int x, int y )
{
  m_camHandler->listenMouseMove(x, y);
}

void App::listenMouseClick( int button, int state, int x, int y )
{
  m_camHandler->listenMouseClick(button, state, x, y);
}

void App::loadArgs()
{
  map<string, string*> :: iterator strIt;
  map<string, int*> :: iterator intIt;
  map<string, float*> :: iterator floatIt;

  for(int i = 1; i < m_argc; ++i)
  {
    if(m_argv[i][0] != '-')
      continue;

    string arg = string(m_argv[i]);

    strIt = m_acceptedArgsString.find(arg);
    if(strIt != m_acceptedArgsString.end())
    {
      if(m_argc > i + 1 && m_argv[i + 1][0] != '-') 
        *(strIt->second) = string(m_argv[i + 1]);
      else cout << "Argument passed to "<< arg << " is invalid!" <<endl;
    }
    

    floatIt = m_acceptedArgsFloat.find(arg);
    if(floatIt != m_acceptedArgsFloat.end())
    {
      if(m_argc > i + 1 && m_argv[i + 1][0] != '-') 
        *(floatIt->second) = atof(m_argv[i + 1]);
      else cout << "Argument passed to "<< arg << " is invalid!" <<endl;
    }

    intIt = m_acceptedArgsInt.find(arg);
    if(intIt != m_acceptedArgsInt.end())
    {
      if(m_argc > i + 1 && m_argv[i + 1][0] != '-') 
        *(intIt->second) = atoi(m_argv[i + 1]);
      else cout << "Argument passed to "<< arg << " is invalid!" <<endl;
    }
  }
}
void App::loadScene()
{
  m_camHandler = new SphereGLCameraHandler(10.f, 0.f, 90.f, 5.f);

  GLLight *minerLight = m_camHandler->getMinerLight();
  minerLight->setAmbientColor(Color(0.0,0.0,0.0));
  minerLight->setDiffuseColor(Color(.8,.8,.8));
  minerLight->setSpecularColor(Color(1.,1.,1.));
  minerLight->setPosition(Vector3(0,100,0));

  m_rtScene = new ScScene(m_scenePath);

  for(int i=0; i<m_rtScene->getNumMeshes();++i)
  {
    ScMesh *m = m_rtScene->getMeshAt(i);
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

  if(m_rtScene->getNumMeshes() == 0)
    cout << "No Mesh Loaded!!" <<endl;

  GLfloat amb [4] = {0,0,0,1};
  Color ambientColor = m_rtScene->getAmbientColor();
  amb[0] = ambientColor.r;
  amb[1] = ambientColor.g;
  amb[2] = ambientColor.b;
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);

  Color sceneClearColor = m_rtScene->getClearColor();
  m_clearColor[0] = sceneClearColor.r;
  m_clearColor[1] = sceneClearColor.g;
  m_clearColor[2] = sceneClearColor.b;

  m_nearPlane = m_rtScene->getCamera()->getNear();
  m_farPlane = m_rtScene->getCamera()->getFar();
  m_fov = m_rtScene->getCamera()->getFovy();
  m_appWidth = m_rtScene->getCamera()->getScreenWidth();
  m_appHeight = m_rtScene->getCamera()->getScreenHeight();
}

void App::loadKernels()
{
  GLfloat *pixels = new GLfloat[m_appWidth*m_appHeight*4];
  for(int i=0; i < m_appWidth*m_appHeight*4; ++i)
    pixels[i] = 0.0;

  GLuint dummyTexId;
  glGenTextures(1, &dummyTexId);
  glBindTexture(GL_TEXTURE_2D, dummyTexId);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); 
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, m_appWidth, m_appHeight, 0, GL_RGBA, GL_FLOAT, pixels);
  glBindTexture(GL_TEXTURE_2D, 0);


  m_kernelDeferred_Peeling = new KernelDeferred_Peeling((char*)m_shaderPath.c_str(), m_appWidth, m_appHeight
    ,dummyTexId ,3
    );

  m_kernelColor = new KernelColor(m_appWidth, m_appHeight);

  m_kernelSSAO = new KernelSSAO((char*)m_shaderPath.c_str(), m_appWidth, m_appHeight
    ,m_kernelDeferred_Peeling->getTexIdPosition(0)
    ,m_kernelDeferred_Peeling->getTexIdNormal(0)
    ,m_kernelDeferred_Peeling->getTexIdNormal(1)
    ,m_kernelDeferred_Peeling->getTexIdNormal(2)
    );

  m_kernelBlur = new KernelBlur((char*)m_shaderPath.c_str(), m_appWidth, m_appHeight, m_kernelSSAO->getColorTexId());
  m_kernelCombine = new KernelCombine((char*)m_shaderPath.c_str(), m_appWidth, m_appHeight, m_kernelColor->getTexIdColor());
}

void App::drawScene()
{
  glPushAttrib(GL_CURRENT_BIT|GL_LIGHTING_BIT);
  m_camHandler->setMinerLightOn(m_minerLight_on);
  m_camHandler->renderMinerLight();

  m_rtScene->setSceneLightEnabled(m_lights_on);
  m_rtScene->configure();
  m_rtScene->render();
  glPopAttrib();
}

void App::renderGUI()
{
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  m_fontRender->setSize(GLFont::Medium);
  m_fontRender->initText(m_appWidth, m_appHeight);

  char a[100];
  int i = 0;
  float x = .70;
  float y = .03;

  sprintf(a,"(F1)%s Menu", m_menu_on?"Close":"Open");
  m_fontRender->print(m_appWidth*x,m_appHeight*y + 25*i++,a, Color(0., 0., 0.));

  if(m_menu_on)
  {
    sprintf(a,"(+/-)%d Num Peelings", m_numPeelings);
    m_fontRender->print(m_appWidth*x,m_appHeight*y + 25*i++,a, Color(0., 0., 0.));

    sprintf(a,"(PgUp/PgDn)rfar: %.2f ", m_rfar);
    m_fontRender->print(m_appWidth*x,m_appHeight*y + 25*i++,a, Color(0., 0., 0.));

    sprintf(a,"(Home/End)pixmask: %.3f ", m_pixelmaskSize);
    m_fontRender->print(m_appWidth*x,m_appHeight*y + 25*i++,a, Color(0., 0., 0.));

    sprintf(a,"(K/J)offsets_size: %.1f ", m_offsetSize);
    m_fontRender->print(m_appWidth*x,m_appHeight*y + 25*i++,a, Color(0., 0., 0.));

    sprintf(a,"(I/U)intensity: %.2f ", m_intensity);
    m_fontRender->print(m_appWidth*x,m_appHeight*y + 25*i++,a, Color(0., 0., 0.));

    sprintf(a,"(F11)Shader %s", m_shader_active? "On":"Off");
    m_fontRender->print(m_appWidth*x,m_appHeight*y + 25*i++,a, Color(0., 0., 0.));

    sprintf(a,"(b)AO Blur %s", m_blurr_on? "On":"Off");
    m_fontRender->print(m_appWidth*x,m_appHeight*y + 25*i++,a, Color(0., 0., 0.));

    sprintf(a,"(l)Lights %s", m_lights_on? "On":"Off");
    m_fontRender->print(m_appWidth*x,m_appHeight*y + 25*i++,a, Color(0., 0., 0.));

    sprintf(a,"(m)Mine Light: %s", m_minerLight_on? "On":"Off");
    m_fontRender->print(m_appWidth*x,m_appHeight*y + 25*i++,a, Color(0., 0., 0.));
  }
  sprintf(a,"%.2f FPS", m_frames->getFPS());
  m_fontRender->print(m_appWidth*.85,m_appHeight*.85+55,a, Color(0., 0., 0.));

  m_fontRender->endText();
  glPopAttrib();
}


