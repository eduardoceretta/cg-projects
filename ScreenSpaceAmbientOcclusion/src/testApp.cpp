/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Feb 2012
 *
 *  Receives a Scene description file and realizes a series of testes based on arguments
 *
 *  Initializes the scene, process the geometry via different kernel
 *  passes, do time tests and screenshots and log results.
 */
#include <iostream>
#include <sstream>
#include <GL\glew.h>
#include <GL\glut.h>
#include <cmath>

#include "MathUtils/Vector3.h"
#include "MathUtils/Matrix3.h"
#include "MathUtils/Matrix4.h"

#include "Handlers/GLCameraHandler.h"
#include "Handlers/SphereGLCameraHandler.h"

#include "ScScene/ScScene.h"
#include "MeshLoaders/P3bMeshFile.h"

#include "GLUtils/GLFont.h"

#include "Kernels/KernelColor.h"
#include "Kernels/KernelDeferred_Peeling.h"
#include "Kernels/KernelBlur.h"
#include "Kernels/KernelCombine.h"
#include "Kernels/KernelVoxDepth.h"
#include "Kernels/KernelVoxelization.h"
#include "Kernels/KernelSSAO_SphereApproximation.h"
#include "Kernels/KernelSSAO_Vox_RayMarch.h"
#include "Kernels/KernelSSAO_Vox_ConeTracing.h"

#include "GLUtils/GLTextureObject.h"

#include "defines.h"
#include "testApp.h"
#include "testTests.h"

#define STR(s) #s


string TestApp::s_renderModeStr[] = {
  STR(NoShader)
  ,STR(Sphere)
  ,STR(RayMarch)
  ,STR(ConeTracing)
  ,STR(Voxelization)
  ,STR(Debug)
};


/*******************************************/
/* App                                     */
/*******************************************/
TestApp::TestApp()
:m_scenePath(APP_DEFAULT_SCENE_PATH)
,m_shaderPath(APP_DEFAULT_SHADER_PATH)

,m_appWidth(APP_DEFAULT_WIDTH)
,m_appHeight(APP_DEFAULT_HEIGHT)

,m_nearPlane(APP_DEFAULT_NEAR)
,m_farPlane(APP_DEFAULT_FAR)
,m_fov(APP_DEFAULT_FOV)

,m_rtScene(NULL)
,m_p3bMesh(NULL)
,m_fontRender(new GLFont())

,m_kernelDeferred_Peeling(NULL)
,m_kernelColor(NULL)
,m_kernelBlur(NULL)
,m_kernelCombine(NULL)
,m_kernelVoxDepth(NULL)
,m_kernelVoxelization(NULL)
,m_kernelSSAO_SphereApproximation(NULL)
,m_kernelSSAO_Vox_RayMarch(NULL)
,m_kernelSSAO_Vox_ConeTracing(NULL)

,m_logPath(APP_DEFAULT_LOG_PATH)
,m_logFileName("")

,m_timeTest(NULL)
,m_timeTestEnabled(false)

,m_screenShotTest(NULL)
,m_screenShotsPath(APP_DEFAULT_SCREEN_TEST_PATH)
,m_screenShotTestEnabled(false)
,m_testCamIndex(0)

,m_Sphere_enabled(false)
,m_Sphere_pixelmaskSize(0.8f)
,m_Sphere_offsetSize(5.0f)
,m_Sphere_numPeelings(3)
,m_Sphere_rfarPercent(30.f)
,m_Sphere_contrast(20.0f)

,m_RayMarch_enabled(false)
,m_RayMarch_rfarPercent(0.06f)
,m_RayMarch_contrast(2.14f)

,m_ConeTracing_enabled(false)
,m_ConeTracing_jitter(true)
,m_ConeTracing_rfarPercent(0.1f)
,m_ConeTracing_contrast(1.28f)
,m_ConeTracing_coneAngle(DEG_TO_RAD(30.0f))
,m_ConeTracing_numCones(6)
,m_ConeTracing_numSpheres(3)
,m_ConeTracing_numSpamplers(3)
,m_ConeTracing_sphereInfoMethod("InitDist")
,m_ConeTracing_sphereInfoSphereOverlap(0.0f)

,m_voxProjectionMatrix(new GLProjectionMatrix())
,m_renderMode(ConeTracing)
{
  m_clearColor[0] =  0.8f;
  m_clearColor[1] =  0.8f;
  m_clearColor[2] =  1.0f;
  m_clearColor[3] = -1.0f;

  m_ambientColor[0] = 0.2f;
  m_ambientColor[1] = 0.2f;
  m_ambientColor[2] = 0.2f;
  m_ambientColor[3] = 1.0f;

  //Initializes Arguments
  m_acceptedArgsString["-scenepath"] = &m_scenePath;
  m_acceptedArgsString["-shaderpath"] = &m_shaderPath;
  
  m_acceptedArgsString["-logPath"] = &m_logPath;
  m_acceptedArgsString["-logFileName"] = &m_logFileName;
    m_acceptedArgsString["-screenShotsPath"] = &m_screenShotsPath;

  m_acceptedArgsBool["-test:screenshot"] = &m_screenShotTestEnabled;
  m_acceptedArgsBool["-test:time"] = &m_timeTestEnabled;

  m_acceptedArgsBool["-alg:Sphere"] = &m_Sphere_enabled;
  m_acceptedArgsBool["-alg:RayMarch"] = &m_RayMarch_enabled;
  m_acceptedArgsBool["-alg:ConeTracing"] = &m_ConeTracing_enabled;

  m_acceptedArgsFloat["-parm:Sphere:rfar"] = &m_Sphere_offsetSize;
  m_acceptedArgsFloat["-parm:Sphere:offset"] = &m_Sphere_offsetSize;
  m_acceptedArgsFloat["-parm:Sphere:contrast"] = &m_Sphere_contrast;

  m_acceptedArgsFloat["-parm:RayMarch:rfar"] = &m_RayMarch_rfarPercent;
  m_acceptedArgsFloat["-parm:RayMarch:contrast"] = &m_RayMarch_contrast;

  m_acceptedArgsBool["-parm:ConeTracing:jitter"] = &m_ConeTracing_jitter;
  m_acceptedArgsFloat["-parm:ConeTracing:rfar"] = &m_ConeTracing_rfarPercent;
  m_acceptedArgsFloat["-parm:ConeTracing:contrast"] = &m_ConeTracing_contrast;
  m_acceptedArgsFloat["-parm:ConeTracing:coneAngle"] = &m_ConeTracing_coneAngle;
  m_acceptedArgsInt["-parm:ConeTracing:numCones"] = &m_ConeTracing_numCones;
  m_acceptedArgsInt["-parm:ConeTracing:numSpheres"] = &m_ConeTracing_numSpheres;
  m_acceptedArgsInt["-parm:ConeTracing:numSamplers"] = &m_ConeTracing_numSpamplers;
  m_acceptedArgsString["-parm:ConeTracing:sphereInfoMethod"] = &m_ConeTracing_sphereInfoMethod;
  m_acceptedArgsFloat["-parm:ConeTracing:sphereInfoSphereOverlap"] = &m_ConeTracing_sphereInfoSphereOverlap;
}

TestApp::~TestApp()
{
  //vector<SphereGLCameraHandler*> :: iterator camHandlersIt;
  //for(camHandlersIt = m_kernelsCamHandleres.begin(); camHandlersIt != m_kernelsCamHandleres.end(); ++camHandlersIt)
  //{
  //  if(*camHandlersIt)
  //  {  
  //    delete *camHandlersIt;
  //    *camHandlersIt = NULL;
  //  }
  //}
  
  if(m_rtScene)
    delete m_rtScene;

  if(m_p3bMesh)
    delete m_p3bMesh;

  if(m_kernelDeferred_Peeling)
    delete m_kernelDeferred_Peeling;

  if(m_kernelColor)
    delete m_kernelColor;

  if(m_kernelBlur)
    delete m_kernelBlur;

  if(m_kernelCombine)
    delete m_kernelCombine;

  if(m_kernelVoxelization)
    delete m_kernelVoxelization;

  if(m_kernelSSAO_SphereApproximation)
    delete m_kernelSSAO_SphereApproximation;

  if(m_kernelSSAO_Vox_RayMarch)
    delete m_kernelSSAO_Vox_RayMarch;

  if(m_kernelSSAO_Vox_ConeTracing)
    delete m_kernelSSAO_Vox_ConeTracing;

  if(m_voxProjectionMatrix)
    delete m_voxProjectionMatrix;

}


void TestApp::initGL(int *argc, char *argv[])
{
  glutInit(argc, argv);
  glewInit();	

  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(m_appWidth, m_appHeight);
  glutInitWindowPosition(100, 100);
  glutCreateWindow(APP_NAME);

  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, m_ambientColor);
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
}

void TestApp::loadParameters(int argc, char *argv[])
{
  loadArgs(argc, argv);
  loadSceneParameters();
}

void TestApp::loadResources()
{
  loadScene();
  loadKernels();
  loadCameras();
  loadTests();
  listenReshape(m_appWidth, m_appHeight);
}


void TestApp::render()
{
  if(!m_screenShotTestEnabled && !m_timeTestEnabled)
  {
    cout << "Done" <<endl;
#ifdef LOG_TESTS
    TestLogger::inst()->closeLog();
#endif // LOG_TESTS
    exit(42);
  }

  int algorithmIndex = m_testCamIndex/(m_rtScene->getNumCameras() - 1);
  
  if(algorithmIndex < m_algorithms.size())
  {
    stringstream ss;
    m_renderMode = m_algorithms[algorithmIndex];
    int poseIndex = m_testCamIndex%(m_rtScene->getNumCameras() - 1);
    
#ifdef LOG_TESTS
    if(poseIndex == 0)
      ss << "[\"" << s_renderModeStr[m_algorithms[algorithmIndex]] << "\"] = { " << endl;
    ss << "  {" << endl;
    ss << "    [\"pose\"] = \"Cam_" << poseIndex << "\"," << endl;
#endif // LOG_TESTS

    m_customCamHandleres[poseIndex]->render();


    //TIME
    //TIME
    //TIME
    //TIME
    if(m_timeTestEnabled)
    {
      switch(m_renderMode)
      {
        default:
        case NoShader:
          break;
        case Sphere:
          m_timeTest->clearProfiler();
          m_timeTest->pushBackStep("sphere", sphere, this);
          m_timeTest->timeProfile();
          break;
        case RayMarch:
          m_timeTest->clearProfiler();
          m_timeTest->pushBackStep("rayMarch", rayMarch, this);
          m_timeTest->timeProfile();
          break;
        case ConeTracing:
          m_timeTest->clearProfiler();
          m_timeTest->pushBackStep("deferredShading", deferredShading, this);
          m_timeTest->pushBackStep("voxelize", voxelize, this);
          m_timeTest->pushBackStep("coneTracing", coneTracing, this);
          m_timeTest->timeProfile();
          break;
      }

#ifdef LOG_TESTS
      ss << "    [\"timeTest\"] = {" << endl;
      ss << m_timeTest->getFormatedResult() << endl;
      ss << "    }," << endl;
#endif // LOG_TESTS
    }
    
    //SCREENSHOT
    //SCREENSHOT
    //SCREENSHOT
    //SCREENSHOT

    if(m_screenShotTestEnabled)
    {
      string screenShotFileName = "";
      switch(m_renderMode)
      {
      default:
      case NoShader:
        break;
      case Sphere:
        screenShotFileName = m_screenShotTest->save(sphere, this, m_kernelCombine->getOutputTexture(0), m_screenShotsPath);
        m_kernelCombine->renderOutput(0);
        break;
      case RayMarch:
        screenShotFileName = m_screenShotTest->save(rayMarch, this, m_kernelSSAO_Vox_RayMarch->getOutputTexture(KernelSSAO_Vox_RayMarch::SSAO), m_screenShotsPath);
        m_kernelSSAO_Vox_RayMarch->renderOutput(KernelSSAO_Vox_RayMarch::SSAO);
        break;
      case ConeTracing:
        deferredShading(this);
        voxelize(this);
        screenShotFileName = m_screenShotTest->save(coneTracing, this, m_kernelSSAO_Vox_ConeTracing->getOutputTexture(KernelSSAO_Vox_ConeTracing::SSAO), m_screenShotsPath);
        m_kernelSSAO_Vox_ConeTracing->renderOutput(KernelSSAO_Vox_ConeTracing::SSAO);
        break;
      }

#ifdef LOG_TESTS
      ss << "    [\"screenshotTest\"] = {" << endl;
      ss << "      [\"screenshotPath\"] = [[" << m_screenShotsPath << "]]," << endl;
      ss << "      [\"screenshotFile\"] = [[" << screenShotFileName << "]]," << endl;
      ss << "    }," << endl;
#endif // LOG_TESTS
    }

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    m_fontRender->setSize(GLFont::Medium);
    m_fontRender->initText(m_appWidth, m_appHeight);

    char a[100];
    int i = 0;
    sprintf(a,"Tests");
    m_fontRender->print(m_appWidth*.45f, m_appHeight*.45 + i++*25,a, Color(0., 0., 0.));

    sprintf(a,"  %s: Cam_%d", s_renderModeStr[m_algorithms[algorithmIndex]].c_str(), poseIndex);
    m_fontRender->print(m_appWidth*.45f, m_appHeight*.45 + i++*25,a, Color(0., 0., 0.));

    m_fontRender->endText();
    glPopAttrib();

#ifdef LOG_TESTS
    ss << "  }, " << endl;
    if(poseIndex == (m_rtScene->getNumCameras() - 1 - 1))
      ss << "}, " << endl;
    TestLogger::inst()->log(ss.str());
#endif // LOG_TESTS

    m_testCamIndex++;
  }else 
  {
    m_screenShotTestEnabled = false;
    m_timeTestEnabled = false;
  }
  
}


void TestApp::listenReshape( int w, int h )
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

void TestApp::loadArgs(int argc, char *argv[])
{
  map<string, string*> :: iterator strIt;
  map<string, int*> :: iterator intIt;
  map<string, float*> :: iterator floatIt;
  map<string, bool*> :: iterator boolIt;

  for(int i = 1; i < argc; ++i)
  {
    if(argv[i][0] != '-')
      continue;

    string arg = string(argv[i]);

    strIt = m_acceptedArgsString.find(arg);
    if(strIt != m_acceptedArgsString.end())
    {
      if(argc > i + 1 && argv[i + 1][0] != '-') 
        *(strIt->second) = string(argv[i + 1]);
      else cout << "Argument passed to "<< arg << " is invalid!" <<endl;
    }

    floatIt = m_acceptedArgsFloat.find(arg);
    if(floatIt != m_acceptedArgsFloat.end())
    {
      if(argc > i + 1 && argv[i + 1][0] != '-') 
        *(floatIt->second) = atof(argv[i + 1]);
      else cout << "Argument passed to "<< arg << " is invalid!" <<endl;
    }

    intIt = m_acceptedArgsInt.find(arg);
    if(intIt != m_acceptedArgsInt.end())
    {
      if(argc > i + 1 && argv[i + 1][0] != '-') 
        *(intIt->second) = atoi(argv[i + 1]);
      else cout << "Argument passed to "<< arg << " is invalid!" <<endl;
    }

    boolIt = m_acceptedArgsBool.find(arg);
    if(boolIt != m_acceptedArgsBool.end())
    {
      if(argc > i + 1 && argv[i + 1][0] != '-') 
        *(boolIt->second) = !strcmp(argv[i + 1], "True") || !strcmp(argv[i + 1], "true");
      else *(boolIt->second) = true; //cout << "Argument passed to "<< arg << " is invalid!" <<endl;
    }
  }

#ifdef LOG_TESTS
  TestLogger::setPath(m_logPath);
  TestLogger::setFileName(m_logFileName);
  TestLogger::inst()->logLine("");
  TestLogger::inst()->logLine("-- Program and argument list:");

  TestLogger::inst()->logLine(string("[\"argument_list\"] = {"));
  TestLogger::inst()->logLine(string("  [\"program_path\"] = [[") + string(argv[0]) + string("]],"));
  for(int i = 1; i < argc; ++i)
  {
    TestLogger::inst()->log(string("  [\"") + string(argv[i]) + string("\"] = "));
    if(argc > i + 1 && argv[i + 1][0] != '-')
      TestLogger::inst()->logLine(string("  [[") + string(argv[++i]) + string("]],"));
    else TestLogger::inst()->logLine(string("  [[true]],"));
  }
  TestLogger::inst()->logLine(string("},"));
#endif // LOG_TESTS
}

void TestApp::loadSceneParameters()
{
  m_rtScene = new ScScene();
  m_rtScene->readSceneParameters(m_scenePath);

  m_appWidth = m_rtScene->getScreenWidth();
  m_appHeight = m_rtScene->getScreenHeight();

  Color ambientColor = m_rtScene->getAmbientColor();
  m_ambientColor[0] = ambientColor.r;
  m_ambientColor[1] = ambientColor.g;
  m_ambientColor[2] = ambientColor.b;

  Color sceneClearColor = m_rtScene->getClearColor();
  m_clearColor[0] = sceneClearColor.r;
  m_clearColor[1] = sceneClearColor.g;
  m_clearColor[2] = sceneClearColor.b;
}

void TestApp::loadScene()
{
  if(!m_rtScene)
    m_rtScene = new ScScene();
  m_rtScene->readSceneObjects(m_scenePath);

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

  cout << "===================================="<<endl;
  cout << "Scene Number of Vertices: " << m_rtScene->getNumVertices() <<endl;
  cout << "Scene Number of Triangles: " << m_rtScene->getNumElements()<<endl;
  cout << "===================================="<<endl;

#ifdef LOG_TESTS
  stringstream ss;
  ss << endl;
  ss << "-- Contents" << endl;
  ss << "[\"resolution\"] = {" << m_appWidth << ", " << m_appHeight << "}," << endl;
  
  ss << "[\"scene\"] = {" << endl;
  ss << "  [\"totalVerices\"] = " << m_rtScene->getNumVertices() << "," << endl;
  ss << "  [\"totalTriangles\"] = " << m_rtScene->getNumElements() << "," <<endl;
  ss << "  [\"models\"] = {" << endl;
  for(int i = 0; i < m_rtScene->getNumMeshes(); ++i)
  {
    ScMesh *m = m_rtScene->getMeshAt(i);
    if(m!=NULL)
    {
      ss << "    {\"" << m->getFileName() << "\", " 
         << m->getNumVertices() << ", " 
         << m->getNumElements() << "}, " << endl;
    }

    P3bMeshFile * p3bMesh2 = m->getP3bMesh();
    if(p3bMesh2!=NULL)
    {
      ss << "    {" << m->getFileName() << ", " 
        << 0 << ", " 
        << 0 << "}, " << endl;
    }
  }
  ss << "  }, " << endl;
  ss << "}, " << endl;
  TestLogger::inst()->log(ss.str());
#endif // LOG_TESTS
}

void TestApp::loadKernels()
{
  if(m_Sphere_enabled)
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

    m_kernelCombine = new KernelCombine((char*)m_shaderPath.c_str(), m_appWidth, m_appHeight
      ,m_kernelColor->getTexIdColor()
      );

    m_kernelSSAO_SphereApproximation = new KernelSSAO_SphereApproximation((char*)m_shaderPath.c_str(), m_appWidth, m_appHeight
      ,m_kernelDeferred_Peeling->getTexIdPosition(0)
      ,m_kernelDeferred_Peeling->getTexIdNormal(0)
      ,m_kernelDeferred_Peeling->getTexIdNormal(1)
      ,m_kernelDeferred_Peeling->getTexIdNormal(2)
      );
  }


  if(m_RayMarch_enabled || m_ConeTracing_enabled)
  {
    m_kernelVoxDepth = new KernelVoxDepth((char*)m_shaderPath.c_str(), m_appWidth, m_appHeight);

    m_kernelVoxelization = new KernelVoxelization((char*)m_shaderPath.c_str(), m_appWidth, m_appHeight 
      ,1 
      ,m_kernelVoxDepth->getTexIdEyePos()
      );
  }

  if(m_RayMarch_enabled)
  {
    m_kernelSSAO_Vox_RayMarch = new KernelSSAO_Vox_RayMarch((char*)m_shaderPath.c_str(), m_appWidth, m_appHeight
      ,m_kernelVoxDepth->getTexIdEyePos()
      ,m_kernelVoxDepth->getTexIdNormalDepth()
      ,m_kernelVoxelization->getTexIdGrid0()
      ,m_kernelVoxelization->getTexIdGridInvFunc()
      );
  }

  if(m_ConeTracing_enabled)
  {
    m_kernelSSAO_Vox_ConeTracing = new KernelSSAO_Vox_ConeTracing((char*)m_shaderPath.c_str(), m_appWidth, m_appHeight
      ,m_kernelVoxDepth->getTexIdEyePos()
      ,m_kernelVoxDepth->getTexIdNormalDepth()
      ,m_kernelVoxelization->getTexIdGrid0()
      ,m_kernelVoxelization->getTexIdGridInvFunc()
      );

    m_kernelSSAO_Vox_ConeTracing->setJitterEnabled(m_ConeTracing_jitter);
    if(m_ConeTracing_sphereInfoMethod != "InitDist")
    {
      m_kernelSSAO_Vox_ConeTracing->getSphereInfo()->nextCalcMethod();
      m_kernelSSAO_Vox_ConeTracing->getSphereInfo()->radProgParms.sphereCenterParm = 2.5f;
      m_kernelSSAO_Vox_ConeTracing->getSphereInfo()->radProgParms.sphereRadiusParm = .5f;
    }else m_kernelSSAO_Vox_ConeTracing->getSphereInfo()->radDistParms.sphereOverlap = m_ConeTracing_sphereInfoSphereOverlap;

    m_kernelSSAO_Vox_ConeTracing->setConeRevolutionAngle(m_ConeTracing_coneAngle);
    m_kernelSSAO_Vox_ConeTracing->setNumCones(m_ConeTracing_numCones);
    m_kernelSSAO_Vox_ConeTracing->setNumSpheresByCone(m_ConeTracing_numSpheres);
    m_kernelSSAO_Vox_ConeTracing->setNumSphereSamplers(m_ConeTracing_numSpamplers);
    m_kernelSSAO_Vox_ConeTracing->setRfarPercent(m_ConeTracing_rfarPercent);
    m_kernelSSAO_Vox_ConeTracing->setContrast(m_ConeTracing_contrast);
  }

  if(m_Sphere_enabled || m_ConeTracing_enabled)
  {
    m_kernelBlur = new KernelBlur((char*)m_shaderPath.c_str(), m_appWidth, m_appHeight
      ,m_Sphere_enabled ? m_kernelSSAO_SphereApproximation->getColorTexId():m_kernelSSAO_Vox_ConeTracing->getTexIdSSAO()
      );
  }
}

void TestApp::loadTests()
{
  if(m_Sphere_enabled)
    m_algorithms.push_back(Sphere);

  if(m_RayMarch_enabled)
    m_algorithms.push_back(RayMarch);

  if(m_ConeTracing_enabled)
     m_algorithms.push_back(ConeTracing);

#ifdef LOG_TESTS
  stringstream ss;
  ss << "[\"algorithms\"] = { " << endl;

  vector<RenderMode> :: iterator it;
  for(it = m_algorithms.begin(); it != m_algorithms.end(); ++it)
    ss << "  \"" << s_renderModeStr[*it] << "\", " << endl;
  ss << "}, " << endl;

  ss << endl;
#endif // LOG_TESTS

  if(m_screenShotTestEnabled)
    m_screenShotTest = new ScreenShotTest();

  if(m_timeTestEnabled)
    m_timeTest = new TimeTest();

#ifdef LOG_TESTS
  ss << "[\"tests\"] = { " << endl;

  if(m_screenShotTestEnabled)
    ss << "  \"screenshotTest\", " << endl;

  if(m_timeTestEnabled)
    ss << "  \"timeTest\", " << endl;

  ss << "}, " << endl;
  ss << "-- Algorithms" << endl;

  TestLogger::inst()->log(ss.str());
#endif // LOG_TESTS
}

void TestApp::loadCameras()
{
  Vector3 bbSize = m_rtScene->getSceneBoundingBoxSize();
  float bbMaxSize = max(bbSize.x, max(bbSize.y, bbSize.z));

  m_fov = m_rtScene->getCamera()->getFovy();

  SphereGLCameraHandler *cam3D = new SphereGLCameraHandler(10.f, 0.f, 90.0f, 5.f);
  cam3D->setViewBoundingBox(m_rtScene->getSceneBoundingBoxMin(), m_rtScene->getSceneBoundingBoxMax(),  m_fov);

  MyAssert("Must exist a custom camera. Number of Camera must be at least 2.\n First for scene setup and others for poses.", 
    m_rtScene->getNumCameras() > 1);
  for(int i = 1; i < m_rtScene->getNumCameras(); ++i)
  {
    ScCamera* scCam = m_rtScene->getCameraAt(i);
    GLCameraHandler *custom = new GLCameraHandler(scCam->getPos(), scCam->getAt(), scCam->getUp());
    m_customCamHandleres.push_back(custom);
  }

  if(cam3D)
  {
    m_nearPlane = cam3D->getSphereRadius()*.05f; 
    m_farPlane = cam3D->getSphereRadius() + bbMaxSize * 1.5f;
  }else
  {
    m_nearPlane = .5;
    m_farPlane  = 10000;
  }

#ifdef LOG_TESTS
  stringstream ss;
  ss << "[\"poses\"] = { " << endl;
  for(int i = 1; i < m_rtScene->getNumCameras(); ++i)
    ss << "  \"Cam_" << i - 1<< "\", " << endl;
  ss << "}, " << endl;

  TestLogger::inst()->log(ss.str());
#endif // LOG_TESTS

}

void TestApp::drawScene()
{
  glPushAttrib(GL_CURRENT_BIT|GL_LIGHTING_BIT);
  m_rtScene->setSceneLightEnabled(false);
  m_rtScene->configure();
  m_rtScene->render();
  glPopAttrib();
}

/************************************************************************/
/* Static Functions                                                     */
/************************************************************************/
void TestApp::deferredShading(TestApp* thisPtr)
{
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //GL_POLYGON_BIT

  thisPtr->m_kernelVoxDepth->setActive(true);
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    thisPtr->drawScene();
  thisPtr->m_kernelVoxDepth->setActive(false);
  glPopAttrib();
}

void TestApp::voxelize(TestApp* thisPtr)
{
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  thisPtr->m_voxProjectionMatrix->readGLProjection();

  thisPtr->m_kernelVoxelization->setActive(true);
    glDisable(GL_CULL_FACE);
    thisPtr->drawScene();
  thisPtr->m_kernelVoxelization->setActive(false);
  glPopAttrib();
}


void TestApp::sphere(TestApp* thisPtr)
{
  //COLOR PASS
  thisPtr->m_kernelColor->setActive(true);
  glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
  glDisable(GL_CULL_FACE);
  thisPtr->drawScene();

  thisPtr->m_kernelColor->setActive(false);

  GLfloat projectionMatrix[16];
  glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);

  //DEPTH PEELING PASS
  for(int i = 0; i < thisPtr->m_Sphere_numPeelings; ++i)
  {
    thisPtr->m_kernelDeferred_Peeling->step(i);
    thisPtr->m_kernelDeferred_Peeling->setActive(true);

    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    glDisable(GL_CULL_FACE);
    thisPtr->drawScene();

    thisPtr->m_kernelDeferred_Peeling->setActive(false);
  }

  //SSAO PASS
  thisPtr->m_kernelSSAO_SphereApproximation->step(projectionMatrix, 
    thisPtr->m_Sphere_rfarPercent, 
    thisPtr->m_Sphere_pixelmaskSize, 
    thisPtr->m_Sphere_offsetSize, 
    thisPtr->m_Sphere_contrast);

  //COMBINE PASS
    thisPtr->m_kernelCombine->step(thisPtr->m_kernelSSAO_SphereApproximation->getColorTexId());
}


void TestApp::rayMarch(TestApp* thisPtr)
{
  thisPtr->deferredShading(thisPtr);
  thisPtr->voxelize(thisPtr);

  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  thisPtr->m_kernelSSAO_Vox_RayMarch->step(thisPtr->m_voxProjectionMatrix, 
    thisPtr->m_RayMarch_rfarPercent, 
    thisPtr->m_RayMarch_contrast);
  glPopAttrib();
}


void TestApp::coneTracing(TestApp* thisPtr)
{
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
 
  thisPtr->m_kernelSSAO_Vox_ConeTracing->step(thisPtr->m_voxProjectionMatrix);

  glPopAttrib();
}



