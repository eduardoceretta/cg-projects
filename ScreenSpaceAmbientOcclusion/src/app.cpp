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
#include "MathUtils/Matrix3.h"
#include "MathUtils/Matrix4.h"

#include "GLUtils/GLFont.h"
#include "GLLights/GLPointLight.h"

#include "Objects/Frames.h"

#include "Handlers/SphereGLCameraHandler.h"

#include "ScScene/ScScene.h"
#include "MeshLoaders/P3bMeshFile.h"

#include "Kernels/KernelColor.h"
#include "Kernels/KernelDeferred_Peeling.h"
#include "Kernels/KernelBlur.h"
#include "Kernels/KernelCombine.h"
#include "Kernels/KernelVoxDepth.h"
#include "Kernels/KernelVoxelization.h"
#include "Kernels/KernelSSAO_SphereApproximation.h"
#include "Kernels/KernelSSAO_HorizonSplit.h"
#include "Kernels/KernelSSAO_Vox_RayMarch.h"
#include "Kernels/KernelSSAO_Vox_TanSphereVolume.h"
#include "Kernels/KernelSSAO_Vox_ConeTracing.h"

#include "GLUtils/GLTextureObject.h"

#include "app.h"

#define STR(s) #s

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
,m_kernelSSAO_SphereApproximation(NULL)
,m_kernelSSAO_HorizonSplit(NULL)
,m_kernelBlur(NULL)
,m_kernelCombine(NULL)
,m_kernelVoxDepth(NULL)
,m_kernelVoxelization(NULL)
,m_kernelSSAO_Vox_RayMarch(NULL)
,m_kernelSSAO_Vox_TanSphereVolume(NULL)
,m_kernelSSAO_Vox_ConeTracing(NULL)
,m_menu_on(false)
,m_lights_on(false)
,m_minerLight_on(false)
,m_updateCamHandler(true)
,m_wireframe_on(false)
,m_voxrender_on(false)
,m_blurr_on(false)
,m_orthographicProjection_on(false)
,m_SSAO_rfarPercent(30.0f)
,m_SSAO_SphereAprox_pixelmaskSize(.8f)
,m_SSAO_SphereAprox_offsetSize(5.0f)
,m_SSAO_contrast(20.0f)
,m_SSAO_SphereAprox_numPeelings(3)
,m_voxProjectionMatrix(new GLProjectionMatrix())
,m_updateVoxelgrid(true)
,m_renderMode(SSAO_Vox_ConeTracing)
{
  m_clearColor[0] = .8f;
  m_clearColor[1] = .8f;
  m_clearColor[2] = 1.0f;
  m_clearColor[3] = -1.;

  //Initializes Arguments
  m_acceptedArgsString["-scenepath"] = &m_scenePath;
  m_acceptedArgsString["-shaderpath"] = &m_shaderPath;

  m_acceptedArgsFloat["-rfar"] = &m_SSAO_rfarPercent;
  m_acceptedArgsFloat["-offset"] = &m_SSAO_SphereAprox_offsetSize;
  m_acceptedArgsFloat["-intensity"] = &m_SSAO_contrast;
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

  if(m_kernelSSAO_SphereApproximation)
    delete m_kernelSSAO_SphereApproximation;

  if(m_kernelSSAO_HorizonSplit)
    delete m_kernelSSAO_HorizonSplit;

  if(m_kernelBlur)
    delete m_kernelBlur;

  if(m_kernelCombine)
    delete m_kernelCombine;

  if(m_kernelVoxelization)
    delete m_kernelVoxelization;

  if(m_kernelSSAO_Vox_RayMarch)
    delete m_kernelSSAO_Vox_RayMarch;

  if(m_kernelSSAO_Vox_TanSphereVolume)
    delete m_kernelSSAO_Vox_TanSphereVolume;

  if(m_kernelSSAO_Vox_ConeTracing)
    delete m_kernelSSAO_Vox_ConeTracing;

  if(m_voxProjectionMatrix)
    delete m_voxProjectionMatrix;

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
  listenReshape(m_appWidth, m_appHeight);
}

//#define RENDER_SAMPLER

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

#ifdef RENDER_SAMPLER
  static int iiii = 0;
  if(!iiii)
    m_camHandler->setViewBoundingBox(Vector3(-1.5f,-.5f,-1.5f), Vector3(1.5f,1.5f,1.5f),  m_fov);
  iiii++;
#endif // RENDER_SAMPLER

  m_frames->update();
  m_camHandler->setMinerLightOn(false);
  m_camHandler->render();

#ifdef RENDER_SAMPLER
  glMatrixMode (GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity ();
  gluPerspective(m_fov, (GLfloat)m_appWidth/(GLfloat)m_appHeight, .0001, 1000.);
  glMatrixMode (GL_MODELVIEW);
  glPushMatrix();
  static int ccounter = 0;

  //m_kernelSSAO_Voxelization->renderRayDistribution(4);
  //m_kernelSSAO_Voxelization_Volume->renderSamplerDistribution(((++ccounter)/100)%5);
  //m_kernelSSAO_Voxelization_Cone->renderSamplerDistribution(((++ccounter)/100)%5);
  m_kernelSSAO_Vox_ConeTracing->renderSamplerDistribution(0);
  //m_kernelSSAO_Voxelization_Cone->renderSphereSamplerDistribution(((++ccounter)/100)%3);
  glPopMatrix();
  glMatrixMode (GL_PROJECTION);
  glPopMatrix();
  glMatrixMode (GL_MODELVIEW);
#else
  switch(m_renderMode)
  {
    default:
    case NoShader:
      renderNoShader();
      break;
    case SSAO_SphereApproximation:
      renderSSAOSphereAproximation();
      break;
    case SSAO_HorizonSplit:
      renderSSAOHorizonSplit();
      break;
    case SSAO_Vox_RayMarch:
      renderSSAOVoxRayMarch();
      break;
    case SSAO_Vox_TanSphereVolume:
      renderSSAOVoxTanSphereVolume();
      break;
    case SSAO_Vox_ConeTracing:
      renderSSAOVoxConeTracing();
      break;
  }
  renderGUI();
#endif // RENDER_SAMPLER
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

  case 'V':
  case 'v':
    if(m_renderMode == SSAO_Vox_RayMarch  && !m_updateVoxelgrid)
    {
      if(m_voxrender_on)
        m_camHandler->setViewBoundingBox(m_rtScene->getSceneBoundingBoxMin(), m_rtScene->getSceneBoundingBoxMax(),  m_fov);
      else m_camHandler->setViewBoundingBox(m_kernelVoxelization->getVoxBBMin(), m_kernelVoxelization->getVoxBBMax(),  m_fov);
      m_voxrender_on = !m_voxrender_on;
    }
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

  case 'O':
  case 'o':
    m_orthographicProjection_on = !m_orthographicProjection_on;
    break;

  case '=':
  case '+':
    m_SSAO_SphereAprox_numPeelings++;
    break;
  case '_':
  case '-':
    m_SSAO_SphereAprox_numPeelings = max(m_SSAO_SphereAprox_numPeelings - 1, 1);
    break;
  case '*':
    m_camHandler->setSphereAlpha(0.0);
    m_camHandler->setSphereBeta(90.0);
    break;

  case 'K':
    m_SSAO_SphereAprox_offsetSize = min(m_SSAO_SphereAprox_offsetSize + 1.0f, m_appWidth/2.0f);
    break;
  case 'k':
    m_SSAO_SphereAprox_offsetSize = min(m_SSAO_SphereAprox_offsetSize + .1f, m_appWidth/2.0f);
    break;
  case 'J':
    m_SSAO_SphereAprox_offsetSize = max(m_SSAO_SphereAprox_offsetSize - 1.0f, 1.0f);
    break;
  case 'j':
    m_SSAO_SphereAprox_offsetSize = max(m_SSAO_SphereAprox_offsetSize - .1f, 1.0f);
    break;

  case 'R':
  case 'r':
    m_kernelDeferred_Peeling->reloadShader();
    m_kernelColor->reloadShader();
    m_kernelSSAO_SphereApproximation->reloadShader();
    m_kernelSSAO_HorizonSplit->reloadShader();
    m_kernelBlur->reloadShader();
    m_kernelCombine->reloadShader();
    m_kernelVoxelization->reloadShader();
    m_kernelSSAO_Vox_RayMarch->reloadShader();
    m_kernelSSAO_Vox_TanSphereVolume->reloadShader();
    m_kernelSSAO_Vox_ConeTracing->reloadShader();
    break;

  
  case 'I':
    m_SSAO_SphereAprox_pixelmaskSize = min(m_SSAO_SphereAprox_pixelmaskSize + .01f * 10 , .999f);
    break;

  case 'i':
    m_SSAO_SphereAprox_pixelmaskSize = min(m_SSAO_SphereAprox_pixelmaskSize + .01f, .999f);
    break;


  case 'U':
    m_SSAO_SphereAprox_pixelmaskSize = max(m_SSAO_SphereAprox_pixelmaskSize - .01f * 10, 0.001f);
    break;
  case 'u':
    m_SSAO_SphereAprox_pixelmaskSize = max(m_SSAO_SphereAprox_pixelmaskSize - .01f, 0.001f);
    break;
  
  
  case '1':
  {
    int step = m_kernelVoxelization->getStepX();
    m_kernelVoxelization->setStepX(min(step + 1, m_appWidth));
    break;
  }
  case '!':
  {
    int step = m_kernelVoxelization->getStepX();
    m_kernelVoxelization->setStepX(max(step - 1, 1));
    break;
  }

  case '2':
  {
    int step = m_kernelVoxelization->getStepY();
    m_kernelVoxelization->setStepY(min(step + 1, m_appHeight));
    break;
  }
  case '@':
  {
    int step = m_kernelVoxelization->getStepY();
    m_kernelVoxelization->setStepY(max(step - 1, 1));
    break;
  }

  case '3':
  {
    int step = m_kernelVoxelization->getStepZ();
    m_kernelVoxelization->setStepZ(min(step + 1, 128));
    break;
  }
  case '#':
  {
    int step = m_kernelVoxelization->getStepZ();
    m_kernelVoxelization->setStepZ(max(step - 1, 1));
    break;
  }

  }
}

void App::listenKeyboardSpecial( int key )
{
  m_camHandler->listenSpecialKeyboard(key);

  int modifier = glutGetModifiers();

  switch(key)
  {
  case 1://F1
    m_menu_on = !m_menu_on;
    break;

  case 3: //F3
    m_SSAO_rfarPercent = 30.0f;
    m_SSAO_contrast = 20.0;

    m_SSAO_SphereAprox_pixelmaskSize = .8;
    m_SSAO_SphereAprox_offsetSize = 5.0;
    
    if(m_renderMode == SSAO_Vox_RayMarch && m_voxrender_on)
      m_camHandler->setViewBoundingBox(m_rtScene->getSceneBoundingBoxMin(), m_rtScene->getSceneBoundingBoxMax(),  m_fov);
    m_renderMode =  SSAO_SphereApproximation;
    break;

  case 4: //F4
    m_SSAO_rfarPercent = .01f;
    m_SSAO_contrast = 1.0;

    if(m_renderMode == SSAO_Vox_RayMarch && m_voxrender_on)
      m_camHandler->setViewBoundingBox(m_rtScene->getSceneBoundingBoxMin(), m_rtScene->getSceneBoundingBoxMax(),  m_fov);
    m_renderMode = SSAO_HorizonSplit;
    break;
  
  case 5: //F5
    if(m_renderMode == SSAO_Vox_RayMarch && m_voxrender_on)
      m_camHandler->setViewBoundingBox(m_rtScene->getSceneBoundingBoxMin(), m_rtScene->getSceneBoundingBoxMax(),  m_fov);
    m_renderMode = SSAO_Vox_RayMarch;
    break;


  case 6: //F6
    m_updateVoxelgrid = true;
    if(m_renderMode == SSAO_Vox_RayMarch && m_voxrender_on)
      m_camHandler->setViewBoundingBox(m_rtScene->getSceneBoundingBoxMin(), m_rtScene->getSceneBoundingBoxMax(),  m_fov);

    m_renderMode = SSAO_Vox_RayMarch;
    break;

  case 7: //F7
    if(m_renderMode == SSAO_Vox_RayMarch && m_voxrender_on)
      m_camHandler->setViewBoundingBox(m_rtScene->getSceneBoundingBoxMin(), m_rtScene->getSceneBoundingBoxMax(),  m_fov);
    m_renderMode = SSAO_Vox_TanSphereVolume;
    break;

  case 8: //F8
    if(m_renderMode == SSAO_Vox_RayMarch && m_voxrender_on)
      m_camHandler->setViewBoundingBox(m_rtScene->getSceneBoundingBoxMin(), m_rtScene->getSceneBoundingBoxMax(),  m_fov);
    m_renderMode = SSAO_Vox_ConeTracing;
    break;

  case 10: //F10
    m_wireframe_on = !m_wireframe_on;

    if(m_wireframe_on)
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    break;

  case 11:
    if(m_renderMode == SSAO_Vox_RayMarch && m_voxrender_on)
      m_camHandler->setViewBoundingBox(m_rtScene->getSceneBoundingBoxMin(), m_rtScene->getSceneBoundingBoxMax(),  m_fov);

    m_renderMode = NoShader;
    break;

  case GLUT_KEY_PAGE_UP:
    m_SSAO_rfarPercent = m_SSAO_rfarPercent + 1. * (modifier == GLUT_ACTIVE_SHIFT ? 1. : .1);
    break;

  case GLUT_KEY_PAGE_DOWN:
    m_SSAO_rfarPercent = max(m_SSAO_rfarPercent - 1. * (modifier == GLUT_ACTIVE_SHIFT ? 1. : .1) , 0.0);
    break;

  case GLUT_KEY_HOME:
    if(modifier == GLUT_ACTIVE_SHIFT)
      m_SSAO_contrast *= 1.1f;
    else 
      m_SSAO_contrast += .05;
    break;

  case GLUT_KEY_END:
    if(modifier == GLUT_ACTIVE_SHIFT)
      m_SSAO_contrast= max(m_SSAO_contrast*.9f, .01f);
    else 
      m_SSAO_contrast = max(m_SSAO_contrast - .05f, .01f);
    break;
  }
}

void App::listenMouseMove( int x, int y )
{
  m_camHandler->listenMouseMove((float)x/m_appWidth, (float)y/m_appHeight);
}

void App::listenMouseClick( int button, int state, int x, int y )
{
  m_camHandler->listenMouseClick(button, state, (float)x/m_appWidth, (float)y/m_appHeight);
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
#ifdef LOG_TESTS
  TestLogger::inst()->logLine("Program and argument list:");
  TestLogger::inst()->logLine(string("  ") + m_argv[0]);
  for(int i = 1; i < m_argc; ++i)
    TestLogger::inst()->log(string("  ") + m_argv[i]);
  TestLogger::inst()->logLine("\nActiveDebugs:");
#ifdef TIME_TEST
  TestLogger::inst()->logLine("  TIME_TEST");
#endif // TIME_TEST
#ifdef SCREENSHOT_TEST
  TestLogger::inst()->logLine("  SCREENSHOT_TEST");
#endif // SCREENSHOT_TEST
  TestLogger::inst()->logLine("\n\n");
#endif // LOG_TESTS
}
void App::loadScene()
{
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

  Vector3 bbSize = m_rtScene->getSceneBoundingBoxSize();
  float bbMaxSize = max(bbSize.x, max(bbSize.y, bbSize.z));

  m_fov = m_rtScene->getCamera()->getFovy();
  m_appWidth = m_rtScene->getCamera()->getScreenWidth();
  m_appHeight = m_rtScene->getCamera()->getScreenHeight();

  m_camHandler = new SphereGLCameraHandler(10.f, 0.f, 90.0f, 5.f);
  m_camHandler->setViewBoundingBox(m_rtScene->getSceneBoundingBoxMin(), m_rtScene->getSceneBoundingBoxMax(),  m_fov);

  m_nearPlane = m_camHandler->getSphereRadius()*.05f; 
  m_farPlane = m_camHandler->getSphereRadius() + bbMaxSize * 1.5f;
  
  //m_nearPlane = m_camHandler->getSphereRadius()*.85; 
  //m_farPlane =  m_camHandler->getSphereRadius()*1.15; 

  //m_nearPlane = m_camHandler->getSphereRadius()*1.0; 
  //m_farPlane =  m_camHandler->getSphereRadius()*1.1; 


  //m_nearPlane = m_camHandler->getSphereRadius()*1.0f; //.1
  //m_farPlane = m_camHandler->getSphereRadius()*.8 + bbMaxSize * .01f; //1.5

  //m_nearPlane = m_camHandler->getSphereRadius();
  //m_farPlane = m_camHandler->getSphereRadius() + bbMaxSize;


  GLLight *minerLight = m_camHandler->getMinerLight();
  minerLight->setAmbientColor(Color(0.0,0.0,0.0));
  minerLight->setDiffuseColor(Color(.8,.8,.8));
  minerLight->setSpecularColor(Color(1.,1.,1.));
  minerLight->setPosition(Vector3(0,100,0));
}

void App::loadKernels()
{
  GLfloat *pixels = new GLfloat[m_appWidth*m_appHeight*4];
  for(int i=0; i < m_appWidth*m_appHeight*4; ++i)
    pixels[i] = 0.0;

  GLTextureObject dummyTex;
  dummyTex.createTexture2D(m_appWidth, m_appHeight, GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT, pixels);
  
  delete[] pixels;

  m_kernelDeferred_Peeling = new KernelDeferred_Peeling((char*)m_shaderPath.c_str(), m_appWidth, m_appHeight
    ,dummyTex.getId(), 3
    );

  m_kernelColor = new KernelColor(m_appWidth, m_appHeight);

  m_kernelSSAO_SphereApproximation = new KernelSSAO_SphereApproximation((char*)m_shaderPath.c_str(), m_appWidth, m_appHeight
    ,m_kernelDeferred_Peeling->getTexIdPosition(0)
    ,m_kernelDeferred_Peeling->getTexIdNormal(0)
    ,m_kernelDeferred_Peeling->getTexIdNormal(1)
    ,m_kernelDeferred_Peeling->getTexIdNormal(2)
    );


  m_kernelSSAO_HorizonSplit = new KernelSSAO_HorizonSplit((char*)m_shaderPath.c_str(), m_appWidth, m_appHeight
    ,m_kernelDeferred_Peeling->getTexIdNormal(0)
    );

  m_kernelBlur = new KernelBlur((char*)m_shaderPath.c_str(), m_appWidth, m_appHeight
    ,m_kernelSSAO_SphereApproximation->getColorTexId()
    );
  m_kernelCombine = new KernelCombine((char*)m_shaderPath.c_str(), m_appWidth, m_appHeight
    ,m_kernelColor->getTexIdColor()
    );

  m_kernelVoxDepth = new KernelVoxDepth((char*)m_shaderPath.c_str(), m_appWidth, m_appHeight);
  m_kernelVoxelization = new KernelVoxelization((char*)m_shaderPath.c_str(), m_appWidth, m_appHeight 
    ,1 
    ,m_kernelVoxDepth->getTexIdEyePos()
    );
  m_kernelSSAO_Vox_RayMarch = new KernelSSAO_Vox_RayMarch((char*)m_shaderPath.c_str(), m_appWidth, m_appHeight
    ,m_kernelVoxDepth->getTexIdEyePos()
    ,m_kernelVoxDepth->getTexIdNormalDepth()
    ,m_kernelVoxelization->getTexIdGrid0()
    ,m_kernelVoxelization->getTexIdGridInvFunc()
    );
  
  m_kernelSSAO_Vox_TanSphereVolume = new KernelSSAO_Vox_TanSphereVolume((char*)m_shaderPath.c_str(), m_appWidth, m_appHeight
    ,m_kernelVoxDepth->getTexIdEyePos()
    ,m_kernelVoxDepth->getTexIdNormalDepth()
    ,m_kernelVoxelization->getTexIdGrid0()
    ,m_kernelVoxelization->getTexIdGridInvFunc()
    );

  m_kernelSSAO_Vox_ConeTracing = new KernelSSAO_Vox_ConeTracing((char*)m_shaderPath.c_str(), m_appWidth, m_appHeight
    ,m_kernelVoxDepth->getTexIdEyePos()
    ,m_kernelVoxDepth->getTexIdNormalDepth()
    ,m_kernelVoxelization->getTexIdGrid0()
    ,m_kernelVoxelization->getTexIdGridInvFunc()
    );
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

  sprintf(a,"(F1) %s Menu", m_menu_on?"Close":"Open");
  m_fontRender->print(m_appWidth*x,m_appHeight*y + 25*i++,a, Color(0., 0., 0.));

  if(m_menu_on)
  {
    sprintf(a,"(F11) Shader %s", m_renderMode == NoShader? "On":"Off");
    m_fontRender->print(m_appWidth*x,m_appHeight*y + 25*i++,a, Color(0., 0., 0.));

    sprintf(a,"(o) Orthographic Proj %s", m_orthographicProjection_on? "On":"Off");
    m_fontRender->print(m_appWidth*x,m_appHeight*y + 25*i++,a, Color(0., 0., 0.));

    sprintf(a,"(l) Lights %s", m_lights_on? "On":"Off");
    m_fontRender->print(m_appWidth*x,m_appHeight*y + 25*i++,a, Color(0., 0., 0.));

    sprintf(a,"(m) Mine Light: %s", m_minerLight_on? "On":"Off");
    m_fontRender->print(m_appWidth*x,m_appHeight*y + 25*i++,a, Color(0., 0., 0.));

    sprintf(a,"(b) Blur %s", m_blurr_on? "On":"Off");
    m_fontRender->print(m_appWidth*x,m_appHeight*y + 25*i++,a, Color(0., 0., 0.));

    sprintf(a,"(PgUp/PgDn) Rfar: %.2f ", m_SSAO_rfarPercent);
    m_fontRender->print(m_appWidth*x,m_appHeight*y + 25*i++,a, Color(0., 0., 0.));

    sprintf(a,"(Home/End) Contrast: %.2f ", m_SSAO_contrast);
    m_fontRender->print(m_appWidth*x,m_appHeight*y + 25*i++,a, Color(0., 0., 0.));
  }

  switch(m_renderMode)
  {
    default:
    case NoShader:
      sprintf(a,"%s", STR(NoShader));
      break;

    case SSAO_SphereApproximation:
      if(m_menu_on)
      {
        sprintf(a,"(+/-)%d Num Peelings", m_SSAO_SphereAprox_numPeelings);
        m_fontRender->print(m_appWidth*x,m_appHeight*y + 25*i++,a, Color(0., 0., 0.));

        sprintf(a,"(I/U) Pixmask: %.3f ", m_SSAO_SphereAprox_pixelmaskSize);
        m_fontRender->print(m_appWidth*x,m_appHeight*y + 25*i++,a, Color(0., 0., 0.));

        sprintf(a,"(K/J) Offsets_size: %.1f ", m_SSAO_SphereAprox_offsetSize);
        m_fontRender->print(m_appWidth*x,m_appHeight*y + 25*i++,a, Color(0., 0., 0.));
      }

      sprintf(a,"%s", STR(SSAO_SphereApproximation));
      break;

    case SSAO_HorizonSplit:
      sprintf(a,"%s", STR(SSAO_HorizonSplit));
      break;

    case SSAO_Vox_RayMarch:
      sprintf(a,"%s", STR(SSAO_Vox_RayMarch));
      break;

    case SSAO_Vox_TanSphereVolume:
      sprintf(a,"%s", STR(SSAO_Vox_TanSphereVolume));
      break;

    case SSAO_Vox_ConeTracing:
      sprintf(a,"%s", STR(SSAO_Vox_ConeTracing));
      break;
  }
  m_fontRender->print(m_appWidth*.03,m_appHeight*.03, a, Color(0., 0., 0.));

  sprintf(a,"%.2f FPS", m_frames->getFPS());
  m_fontRender->print(m_appWidth*.85,m_appHeight*.85+55,a, Color(0., 0., 0.));

  m_fontRender->endText();
  glPopAttrib();
}

void App::renderNoShader()
{
  if(m_orthographicProjection_on)
  {
    glMatrixMode (GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity ();
    Vector3 size = m_rtScene->getSceneBoundingBoxSize();
    float orthoSize = max(max(size.x, size.y), size.z);
    glOrtho(-orthoSize, orthoSize, -orthoSize, orthoSize, m_nearPlane, m_farPlane);

    glMatrixMode (GL_MODELVIEW);
    glPushMatrix();
  }
  drawScene();
  if(m_orthographicProjection_on)
  {
    glPopMatrix();
    glMatrixMode (GL_PROJECTION);
    glPopMatrix();
    glMatrixMode (GL_MODELVIEW);
  }
}



void App::renderSSAOSphereAproximation()
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
  for(int i=0; i < m_SSAO_SphereAprox_numPeelings; ++i)
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
  m_kernelSSAO_SphereApproximation->step(projectionMatrix, m_SSAO_rfarPercent, m_SSAO_SphereAprox_pixelmaskSize,m_SSAO_SphereAprox_offsetSize, m_SSAO_contrast);
#ifdef TIME_TEST
  timeTest.kSSAOTime += timeTest.getTime();
#endif

#ifdef TIME_TEST
  timeTest.resetTimer();
#endif
  //BLURR PASS
  if(m_blurr_on)
  {
    m_kernelBlur->setInputTexId(m_kernelSSAO_SphereApproximation->getColorTexId());
    m_kernelBlur->step(1);
  }
#ifdef TIME_TEST
  timeTest.kBlurTime += timeTest.getTime();
#endif

#ifdef TIME_TEST
  timeTest.resetTimer();
#endif
  //COMBINE PASS
  if(m_blurr_on)
    m_kernelCombine->step(m_kernelBlur->getBlurredTexId());
  else
    m_kernelCombine->step(m_kernelSSAO_SphereApproximation->getColorTexId());
#ifdef TIME_TEST
  timeTest.kCombineTime += timeTest.getTime();
#endif

#ifdef TIME_TEST
  timeTest.totalTime += timeTest.totalTimer.getTime();
  //timeTest.printPartialResults();
#endif
  //RENDER RESULT
  m_kernelCombine->renderOutput(0);
}

void App::renderSSAOHorizonSplit()
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
  for(int i=0; i < m_SSAO_SphereAprox_numPeelings; ++i)
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
  m_kernelSSAO_HorizonSplit->step(projectionMatrix, m_SSAO_rfarPercent, m_SSAO_SphereAprox_pixelmaskSize,m_SSAO_SphereAprox_offsetSize, m_SSAO_contrast);
#ifdef TIME_TEST
  timeTest.kSSAOTime += timeTest.getTime();
#endif

#ifdef TIME_TEST
  timeTest.resetTimer();
#endif
  //BLURR PASS
  if(m_blurr_on)
  {
    m_kernelBlur->setInputTexId(m_kernelSSAO_HorizonSplit->getColorTexId());
    m_kernelBlur->step(1);
  }
#ifdef TIME_TEST
  timeTest.kBlurTime += timeTest.getTime();
#endif

#ifdef TIME_TEST
  timeTest.resetTimer();
#endif
  //COMBINE PASS
  if(m_blurr_on)
    m_kernelCombine->step(m_kernelBlur->getBlurredTexId());
  else
    m_kernelCombine->step(m_kernelSSAO_HorizonSplit->getColorTexId());

#ifdef TIME_TEST
  timeTest.kCombineTime += timeTest.getTime();
#endif

#ifdef TIME_TEST
  timeTest.totalTime += timeTest.totalTimer.getTime();
  //timeTest.printPartialResults();
#endif
  //RENDER RESULT
  m_kernelCombine->renderOutput(0);
}

void App::renderSSAOVoxRayMarch()
{
  if(m_updateVoxelgrid || !m_voxrender_on)
  {
    if(m_orthographicProjection_on)
    {
      glMatrixMode (GL_PROJECTION);
      glPushMatrix();
      glLoadIdentity ();

      Vector3 size = m_rtScene->getSceneBoundingBoxSize();
      float orthoSize = max(max(size.x, size.y), size.z);
      glOrtho(-orthoSize, orthoSize, -orthoSize, orthoSize, m_nearPlane, m_farPlane);
      glMatrixMode (GL_MODELVIEW);
      glPushMatrix();
    }
    glPushAttrib(GL_POLYGON_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //GL_POLYGON_BIT
    m_kernelVoxDepth->setActive(true);

    glClearColor(-1,-1,-1,-1);
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    drawScene();
    m_kernelVoxDepth->setActive(false);
    
    m_voxProjectionMatrix->readGLProjection();

    m_kernelVoxelization->setActive(true);
    drawScene();
    m_kernelVoxelization->setActive(false);

    glPopAttrib();

    if(m_orthographicProjection_on)
    {
      glPopMatrix();
      glMatrixMode (GL_PROJECTION);
      glPopMatrix();
      glMatrixMode (GL_MODELVIEW);
    }

    //m_kernelVoxDepth->renderOutput(0);
    //m_kernelVoxelization->renderOutput(2);

    
    //GLTextureObject texObj;
    //texObj = GLTextureObject(m_kernelVoxDepth->getOutputTexture(2));
    //texObj = GLTextureObject(m_kernelVoxelization->getOutputTexture(1));
    //GLuint * i = texObj.read2DTextureUIntData();
    //GLTextureObject t2 = GLTextureObject(m_kernelVoxelization->getOutputTexture(2));
    //GLTextureObject t2 = GLTextureObject(m_kernelVoxDepth->getOutputTexture(1));
    //GLfloat* f = t2.read2DTextureFloatData();
    //GLuint * d = t2.read2DTextureUIntData();

    m_updateVoxelgrid = false;
    if(m_voxrender_on)
      m_camHandler->setViewBoundingBox(m_kernelVoxelization->getVoxBBMin(), m_kernelVoxelization->getVoxBBMax(),  m_fov);
  }else if(m_voxrender_on)
  {
    glMatrixMode (GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity ();
    gluPerspective(m_fov, (GLfloat)m_appWidth/(GLfloat)m_appHeight, .0001, 1000.);
    glMatrixMode (GL_MODELVIEW);
    glPushMatrix();

    glPushAttrib(GL_CURRENT_BIT|GL_LIGHTING_BIT);

    m_camHandler->setMinerLightOn(m_minerLight_on);
    m_camHandler->renderMinerLight();
    m_rtScene->setSceneLightEnabled(m_lights_on);
    m_rtScene->setMaterialActive(true, 2);
    m_rtScene->setLightActive(true);

    m_kernelVoxelization->renderVoxelization();

    m_rtScene->setLightActive(false);
    m_rtScene->setMaterialActive(false, 2);

    glPopAttrib();

    glPopMatrix();
    glMatrixMode (GL_PROJECTION);
    glPopMatrix();
    glMatrixMode (GL_MODELVIEW);
  }

  if(!m_voxrender_on)
  {
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    m_kernelSSAO_Vox_RayMarch->step(m_voxProjectionMatrix);
    m_kernelSSAO_Vox_RayMarch->renderOutput(KernelSSAO_Vox_RayMarch::SSAO);

    //GLTextureObject texObj = GLTextureObject(m_kernelSSAO_Voxelization->getOutputTexture(1));
    //GLuint* i = texObj.read2DTextureUIntData();
    //GLTextureObject t2 = GLTextureObject(m_kernelSSAO_Voxelization->getOutputTexture(0));
    //GLfloat* f = t2.read2DTextureFloatData();
    glPopAttrib();
  }
}


void App::renderSSAOVoxTanSphereVolume()
{
  {
    if(m_orthographicProjection_on)
    {
      glMatrixMode (GL_PROJECTION);
      glPushMatrix();
      glLoadIdentity ();

      Vector3 size = m_rtScene->getSceneBoundingBoxSize();
      float orthoSize = max(max(size.x, size.y), size.z);
      glOrtho(-orthoSize, orthoSize, -orthoSize, orthoSize, m_nearPlane, m_farPlane);
      glMatrixMode (GL_MODELVIEW);
      glPushMatrix();
    }
    glPushAttrib(GL_POLYGON_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //GL_POLYGON_BIT
    m_kernelVoxDepth->setActive(true);
      glClearColor(-1,-1,-1,-1);
      glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
      drawScene();
    m_kernelVoxDepth->setActive(false);

    m_voxProjectionMatrix->readGLProjection();

    m_kernelVoxelization->setActive(true);
      drawScene();
    m_kernelVoxelization->setActive(false);

    glPopAttrib();

    if(m_orthographicProjection_on)
    {
      glPopMatrix();
      glMatrixMode (GL_PROJECTION);
      glPopMatrix();
      glMatrixMode (GL_MODELVIEW);
    }
  }

  {
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    m_kernelSSAO_Vox_TanSphereVolume->step(m_voxProjectionMatrix);
    m_kernelSSAO_Vox_TanSphereVolume->renderOutput(KernelSSAO_Vox_RayMarch::SSAO);

    //GLTextureObject texObj = GLTextureObject(m_kernelSSAO_Voxelization_Volume->getOutputTexture(1));
    //GLuint* i = texObj.read2DTextureUIntData();
    //GLTextureObject t2 = GLTextureObject(m_kernelSSAO_Voxelization_Volume->getOutputTexture(0));
    //GLfloat* f = t2.read2DTextureFloatData();

    glPopAttrib();
  }
}


void App::renderSSAOVoxConeTracing()
{
  {
    if(m_orthographicProjection_on)
    {
      glMatrixMode (GL_PROJECTION);
      glPushMatrix();
      glLoadIdentity ();

      Vector3 size = m_rtScene->getSceneBoundingBoxSize();
      float orthoSize = max(max(size.x, size.y), size.z);
      glOrtho(-orthoSize, orthoSize, -orthoSize, orthoSize, m_nearPlane, m_farPlane);
      glMatrixMode (GL_MODELVIEW);
      glPushMatrix();
    }
    glPushAttrib(GL_POLYGON_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //GL_POLYGON_BIT
    m_kernelVoxDepth->setActive(true);
    glClearColor(-1,-1,-1,-1);
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    drawScene();
    m_kernelVoxDepth->setActive(false);

    m_voxProjectionMatrix->readGLProjection();

    m_kernelVoxelization->setActive(true);
    drawScene();
    m_kernelVoxelization->setActive(false);

    glPopAttrib();

    if(m_orthographicProjection_on)
    {
      glPopMatrix();
      glMatrixMode (GL_PROJECTION);
      glPopMatrix();
      glMatrixMode (GL_MODELVIEW);
    }
  }

  {
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    m_kernelSSAO_Vox_ConeTracing->step(m_voxProjectionMatrix, m_SSAO_rfarPercent, m_SSAO_contrast);
    m_kernelSSAO_Vox_ConeTracing->renderOutput(KernelSSAO_Vox_RayMarch::SSAO);

    GLTextureObject texObj = GLTextureObject(m_kernelVoxelization->getTexIdGrid0());
    GLuint* i = texObj.read2DTextureUIntData();
    GLTextureObject t2 = GLTextureObject(m_kernelSSAO_Vox_ConeTracing->getOutputTexture(0));
    GLfloat* f = t2.read2DTextureFloatData();

    glPopAttrib();
  }
}



