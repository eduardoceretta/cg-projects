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

#include "Handlers/GLCameraHandler.h"
#include "Handlers/SphereGLCameraHandler.h"

#include "ScScene/ScScene.h"
#include "MeshLoaders/P3bMeshFile.h"
#include "ScScene/ScAnimMesh.h"

#include "Kernels/KernelColor.h"
#include "Kernels/KernelDeferred_Peeling.h"
#include "Kernels/KernelBlur.h"
#include "Kernels/KernelCombine.h"
#include "Kernels/KernelDeferredLighting.h"
#include "Kernels/KernelVoxDepth.h"
#include "Kernels/KernelVoxelization.h"
#include "Kernels/KernelSSAO_SphereApproximation.h"
#include "Kernels/KernelSSAO_HorizonSplit.h"
#include "Kernels/KernelSSAO_Vox_RayMarch.h"
#include "Kernels/KernelSSAO_Vox_TanSphereVolume.h"
#include "Kernels/KernelSSAO_Vox_ConeTracing.h"

#include "GLUtils/GLTextureObject.h"
#include "GLUtils/GLOcclusionQuery.h"

#include "app.h"

#define STR(s) #s


string App::s_renderModeStr[] = {
  STR(NoShader)
  ,STR(SSAO_SphereApproximation)
  ,STR(SSAO_HorizonSplit)
  ,STR(SSAO_Vox_RayMarch)
  ,STR(SSAO_Vox_TanSphereVolume)
  ,STR(SSAO_Vox_ConeTracing)
  ,STR(Voxelization)
  ,STR(Debug)
};



/*******************************************/
/* App                                     */
/*******************************************/
App::App()
:m_scenePath(APP_DEFAULT_SCENE_PATH)
,m_shaderPath(APP_DEFAULT_SHADER_PATH)
,m_appWidth(APP_DEFAULT_WIDTH)
,m_appHeight(APP_DEFAULT_HEIGHT)
,m_nearPlane(APP_DEFAULT_NEAR)
,m_farPlane(APP_DEFAULT_FAR)
,m_fov(APP_DEFAULT_FOV)
,m_camHandler(NULL)
,m_customCameraIndex(0)
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
,m_kernelDeferredLighting(NULL)
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
,m_debugrender_on(false)
,m_diffuseEnabled(false)
,m_aoEnabled(true)
,m_blurr_on(false)
,m_orthographicProjection_on(false)
,m_SSAO_rfarPercent(.1f)
,m_SSAO_pixelmaskSize(.8f)
,m_SSAO_offsetSize(5.0f)
,m_SSAO_contrast(1.28f)
,m_SSAO_jitter(true)
,m_SSAO_numPeelings(3)
,m_SSAO_cone_angle(DEG_TO_RAD(30.0f))
,m_SSAO_cone_numCones(6)
,m_SSAO_cone_numSpheres(3)
,m_SSAO_cone_numSpamplers(3)
,m_SSAO_cone_infoMethod("InitDist")
,m_SSAO_cone_infoSphereOverlap(0.0f)
,m_SSAO_cone_infoCenterParm(2.5f)
,m_SSAO_cone_infoRadiusParm(0.5f)
,m_voxProjectionMatrix(new GLProjectionMatrix())
,m_updateVoxelgrid(true)
,m_renderMode(SSAO_Vox_ConeTracing)
,m_occlusionQuery(NULL)
,m_occlusionQueryEnabled(false)
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

  m_acceptedArgsFloat["-rfar"] = &m_SSAO_rfarPercent;
  m_acceptedArgsFloat["-offset"] = &m_SSAO_offsetSize;
  m_acceptedArgsFloat["-contrast"] = &m_SSAO_contrast;
  m_acceptedArgsBool["-jitter"] = &m_SSAO_jitter;

  m_acceptedArgsFloat["-Cone_angle"] = &m_SSAO_cone_angle;
  m_acceptedArgsInt["-Cone_numCones"] = &m_SSAO_cone_numCones;
  m_acceptedArgsInt["-Cone_numSpheres"] = &m_SSAO_cone_numSpheres;
  m_acceptedArgsInt["-Cone_numSamplers"] = &m_SSAO_cone_numSpamplers;
  
  m_acceptedArgsString["-Cone_infoMethod"] = &m_SSAO_cone_infoMethod;
  m_acceptedArgsFloat["-Cone_infoSphereOverlap"] = &m_SSAO_cone_infoSphereOverlap;
  m_acceptedArgsFloat["-Cone_infoSphereCenterParm"] = &m_SSAO_cone_infoCenterParm;
  m_acceptedArgsFloat["-Cone_infoSphereRadiusParm"] = &m_SSAO_cone_infoRadiusParm;

  //-rfar .1 -contrast 1.28 -jitter False -Cone_angle  0.523598776  -Cone_numCones 9 -Cone_numSpheres 9 -Cone_numSamplers 5 -Cone_infoMethod InitDist -Cone_infoSphereOverlap .5 -Cone_infoSphereCenterParm 2.5 - Cone_infoSphereRadiusParm .5
}

App::~App()
{
  delete m_fontRender;
  delete m_frames;

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
}


void App::initGL(int *argc, char *argv[])
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

void App::loadParameters(int argc, char *argv[])
{
  loadArgs(argc, argv);
  loadSceneParameters();
}

void App::loadResources()
{
  loadScene();
  loadKernels();
  loadCameras();
  listenReshape(m_appWidth, m_appHeight);
}


void App::render()
{
  m_frames->update();
  m_camHandler->setMinerLightOn(false);
  m_camHandler->render();
  

  if(m_debugrender_on)
  {
    glMatrixMode (GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity ();
    gluPerspective(m_fov, (GLfloat)m_appWidth/(GLfloat)m_appHeight, .0001, 1000.);
    glMatrixMode (GL_MODELVIEW);
    glPushMatrix();

    static int ccounter = 0;
    switch(m_renderMode)
    {
    default:
    case NoShader:
    case SSAO_SphereApproximation:
    case SSAO_HorizonSplit:
      break;
    case SSAO_Vox_RayMarch:
      m_kernelSSAO_Vox_RayMarch->renderRayDistribution(4);
      break;
    case SSAO_Vox_TanSphereVolume:
      m_kernelSSAO_Vox_TanSphereVolume->renderSamplerDistribution(((++ccounter)/100)%5);
      break;
    case SSAO_Vox_ConeTracing:
      //m_kernelSSAO_Vox_ConeTracing->renderSphereInfoDistribution(((ccounter)/200)%5);
      //m_kernelSSAO_Vox_ConeTracing->renderSphereInfoDistribution(0);
      m_kernelSSAO_Vox_ConeTracing->renderConeDistribution(0);
      //m_kernelSSAO_Vox_ConeTracing->renderSphereSamplerDistribution(((++ccounter)/200)%5, ((ccounter)/1000)%3);
      break;
    case Voxelization:
      renderVoxelization();
      break;
    }
    glPopMatrix();
    glMatrixMode (GL_PROJECTION);
    glPopMatrix();
    glMatrixMode (GL_MODELVIEW);
  }else
  {
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
    case Voxelization:
      renderVoxelization();
      break;
    }
  }

  renderGUI();
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

  case 'A':
  case 'a':
    {
      Vector3 pos = m_camHandler->getPos();
      Vector3 at = m_camHandler->getAt();
      Vector3 up = m_camHandler->getUp();
      cout << "CAMERA " << pos.x << " " << pos.y << " " << pos.z << "   " 
                       << at.x << " " << at.y << " " << at.z << "   " 
                       << up.x << " " << up.y << " " << up.z << "   " 
                       << m_fov << " " << m_nearPlane  << " " << m_farPlane << endl << endl;
    }
    break;


  case 'S':
  case 's':
    m_occlusionQueryEnabled = !m_occlusionQueryEnabled;
    break;

  case 'C':
    m_customCameraIndex = (m_customCameraIndex - 1 >= 0 ? m_customCameraIndex : m_rtScene->getNumCameras() - 1) - 1;
    m_camHandler = m_kernelsCamHandleres[CustomCameras + m_customCameraIndex];
    break;
  case 'c':
    m_customCameraIndex = (m_customCameraIndex + 1) % (m_rtScene->getNumCameras() - 1);
    m_camHandler = m_kernelsCamHandleres[CustomCameras + m_customCameraIndex];
    break;

  case 'D':
  case 'd':
    m_debugrender_on = !m_debugrender_on;
    m_camHandler = m_kernelsCamHandleres[(m_debugrender_on?Debug:0) + m_renderMode];
    break;

  case 'M':
  case 'm':
    m_minerLight_on = !m_minerLight_on;
    break;

  case 'N':
  case 'n':
    m_SSAO_jitter = !m_SSAO_jitter;
    m_kernelSSAO_Vox_ConeTracing->setJitterEnabled(m_SSAO_jitter);
    break;

  case 'L':
  case 'l':
    m_lights_on = !m_lights_on;
    break;

  case 'B':
  case 'b':
    m_blurr_on = !m_blurr_on;
    break;

  case 'Z':
  case 'z':
    m_aoEnabled = !m_aoEnabled;
    break;

  case 'X':
  case 'x':
    m_diffuseEnabled = !m_diffuseEnabled;
    break;

  case 'O':
  case 'o':
    m_orthographicProjection_on = !m_orthographicProjection_on;
    break;

  case '=':
  case '+':
    m_SSAO_numPeelings++;
    break;
  case '_':
  case '-':
    m_SSAO_numPeelings = max(m_SSAO_numPeelings - 1, 1);
    break;
  case '*':
    {
      SphereGLCameraHandler* p = dynamic_cast<SphereGLCameraHandler*>(m_camHandler);
      if(p)
      {
        p->setSphereAlpha(0.0);
        p->setSphereBeta(90.0);
      }
    }
    break;

  case 'K':
    switch(m_renderMode)
    {
    default:
      break;
    case SSAO_SphereApproximation:
      m_SSAO_offsetSize = min(m_SSAO_offsetSize + 1.0f, m_appWidth/2.0f);
      break;
    case SSAO_Vox_ConeTracing:
      int numSpheres = m_kernelSSAO_Vox_ConeTracing->getNumSpheresByCone();
      numSpheres = ceil(numSpheres * 1.1);
      m_kernelSSAO_Vox_ConeTracing->setNumSpheresByCone(numSpheres);
      break;
    }
    
    break;
  case 'k':
    switch(m_renderMode)
    {
    default:
      break;
    case SSAO_SphereApproximation:
      m_SSAO_offsetSize = min(m_SSAO_offsetSize + .1f, m_appWidth/2.0f);
      break;
    case SSAO_Vox_ConeTracing:
      int numSpheres = m_kernelSSAO_Vox_ConeTracing->getNumSpheresByCone();
      numSpheres += 1;
      m_kernelSSAO_Vox_ConeTracing->setNumSpheresByCone(numSpheres);
      break;
    }
    break;

  case 'J':
    switch(m_renderMode)
    {
    default:
      break;
    case SSAO_SphereApproximation:
      m_SSAO_offsetSize = max(m_SSAO_offsetSize - 1.0f, 1.0f);
      break;
    case SSAO_Vox_ConeTracing:
      int numSpheres = m_kernelSSAO_Vox_ConeTracing->getNumSpheresByCone();
      numSpheres = max(int(floor(numSpheres*.9)), 1);
      m_kernelSSAO_Vox_ConeTracing->setNumSpheresByCone(numSpheres);
      break;
    }
    break;
  case 'j':
    switch(m_renderMode)
    {
    default:
      break;
    case SSAO_SphereApproximation:
      m_SSAO_offsetSize = max(m_SSAO_offsetSize - .1f, 1.0f);
      break;
    case SSAO_Vox_ConeTracing:
      int numSpheres = m_kernelSSAO_Vox_ConeTracing->getNumSpheresByCone();
      numSpheres = max(numSpheres - 1, 1);
      m_kernelSSAO_Vox_ConeTracing->setNumSpheresByCone(numSpheres);
      break;
    }
    break;

  case 'R':
  case 'r':
    m_kernelDeferred_Peeling->reloadShader();
    m_kernelColor->reloadShader();
    m_kernelSSAO_SphereApproximation->reloadShader();
    m_kernelSSAO_HorizonSplit->reloadShader();
    m_kernelBlur->reloadShader();
    m_kernelCombine->reloadShader();
    m_kernelDeferredLighting->reloadShader();
    m_kernelVoxelization->reloadShader();
    m_kernelSSAO_Vox_RayMarch->reloadShader();
    m_kernelSSAO_Vox_TanSphereVolume->reloadShader();
    m_kernelSSAO_Vox_ConeTracing->reloadShader();
    break;

  
  case 'I':
    switch(m_renderMode)
    {
    default:
      break;
    case SSAO_SphereApproximation:
      m_SSAO_pixelmaskSize = min(m_SSAO_pixelmaskSize + .01f * 10 , .999f);
      break;
    case SSAO_Vox_ConeTracing:
      int numCones = m_kernelSSAO_Vox_ConeTracing->getNumCones();
      numCones = ceil(numCones * 1.1);
      m_kernelSSAO_Vox_ConeTracing->setNumCones(numCones);
      break;
    }
    
    break;

  case 'i':
    switch(m_renderMode)
    {
    default:
      break;
    case SSAO_SphereApproximation:
      m_SSAO_pixelmaskSize = min(m_SSAO_pixelmaskSize + .01f, .999f);
      break;
    case SSAO_Vox_ConeTracing:
      int numCones = m_kernelSSAO_Vox_ConeTracing->getNumCones();
      numCones += 1;
      m_kernelSSAO_Vox_ConeTracing->setNumCones(numCones);
      break;
    }
    break;


  case 'U':
    switch(m_renderMode)
    {
    default:
      break;
    case SSAO_SphereApproximation:
      m_SSAO_pixelmaskSize = max(m_SSAO_pixelmaskSize - .01f * 10, 0.001f);
      break;
    case SSAO_Vox_ConeTracing:
      int numCones = m_kernelSSAO_Vox_ConeTracing->getNumCones();
      numCones = max(int(floor(numCones*.5)), 1);
      m_kernelSSAO_Vox_ConeTracing->setNumCones(numCones);
      break;
    }
    break;
  case 'u':
    switch(m_renderMode)
    {
    default:
      break;
    case SSAO_SphereApproximation:
      m_SSAO_pixelmaskSize = max(m_SSAO_pixelmaskSize - .01f, 0.001f);
      break;
    case SSAO_Vox_ConeTracing:
      int numCones = m_kernelSSAO_Vox_ConeTracing->getNumCones();
      numCones = max(numCones - 2, 1);
      m_kernelSSAO_Vox_ConeTracing->setNumCones(numCones);
      break;
    }
    break;


  case 'H':
    switch(m_renderMode)
    {
    default:
      break;
    case SSAO_Vox_ConeTracing:
      int numSphereSamplers = m_kernelSSAO_Vox_ConeTracing->getNumberSphereSamplers();
      numSphereSamplers = ceil(numSphereSamplers * 1.1);
      m_kernelSSAO_Vox_ConeTracing->setNumSphereSamplers(numSphereSamplers);
      break;
    }

    break;

  case 'h':
    switch(m_renderMode)
    {
    default:
      break;
    case SSAO_Vox_ConeTracing:
      int numSphereSamplers = m_kernelSSAO_Vox_ConeTracing->getNumberSphereSamplers();
      numSphereSamplers += 1;
      m_kernelSSAO_Vox_ConeTracing->setNumSphereSamplers(numSphereSamplers);
      break;
    }
    break;


  case 'G':
    switch(m_renderMode)
    {
    default:
      break;
    case SSAO_Vox_ConeTracing:
      int numSphereSamplers = m_kernelSSAO_Vox_ConeTracing->getNumberSphereSamplers();
      numSphereSamplers = max(int(floor(numSphereSamplers*.5)), 1);
      m_kernelSSAO_Vox_ConeTracing->setNumSphereSamplers(numSphereSamplers);
      break;
    }
    break;
  case 'g':
    switch(m_renderMode)
    {
    default:
      break;
    case SSAO_Vox_ConeTracing:
      int numSphereSamplers = m_kernelSSAO_Vox_ConeTracing->getNumberSphereSamplers();
      numSphereSamplers = max(numSphereSamplers - 2, 1);
      m_kernelSSAO_Vox_ConeTracing->setNumSphereSamplers(numSphereSamplers);
      break;
    }
    break;


  case '8':
    switch(m_renderMode)
    {
    default:
      break;
    case SSAO_Vox_ConeTracing:
      float coneAngle = m_kernelSSAO_Vox_ConeTracing->getConeRevolutionAngle();
      coneAngle = DEG_TO_RAD(min((RAD_TO_DEG(coneAngle) + 15.0f),60.0f));
      m_kernelSSAO_Vox_ConeTracing->setConeRevolutionAngle(coneAngle);
      break;
    }
    break;


  case '7':
    switch(m_renderMode)
    {
    default:
      break;
    case SSAO_Vox_ConeTracing:
      float coneAngle = m_kernelSSAO_Vox_ConeTracing->getConeRevolutionAngle();
      coneAngle = DEG_TO_RAD(max((RAD_TO_DEG(coneAngle) - 15.0f), 15.0f));
      m_kernelSSAO_Vox_ConeTracing->setConeRevolutionAngle(coneAngle);
      break;
    }
    break;


  case '(':
    switch(m_renderMode)
    {
    default:
      break;
    case SSAO_Vox_ConeTracing:
      if(m_SSAO_cone_infoMethod != "InitDist")
      {
        float overlap = m_kernelSSAO_Vox_ConeTracing->getSphereInfo()->radDistParms.sphereOverlap;
        overlap = max((overlap*.75f),0.0f);
        m_kernelSSAO_Vox_ConeTracing->getSphereInfo()->radDistParms.sphereOverlap = overlap;
        m_kernelSSAO_Vox_ConeTracing->reloadShaderInput();
      }else
      {
        float center = m_kernelSSAO_Vox_ConeTracing->getSphereInfo()->radProgParms.sphereCenterParm;
        center = max((center - .1f),0.0f);
        m_kernelSSAO_Vox_ConeTracing->getSphereInfo()->radProgParms.sphereCenterParm = center;
        m_kernelSSAO_Vox_ConeTracing->reloadShaderInput();
      }
      break;
    }
    break;
  case '9':
    switch(m_renderMode)
    {
    default:
      break;
    case SSAO_Vox_ConeTracing:
      if(m_SSAO_cone_infoMethod != "InitDist")
      {
        float overlap = m_kernelSSAO_Vox_ConeTracing->getSphereInfo()->radDistParms.sphereOverlap;
        overlap = max((overlap - .01f),0.0f);
        m_kernelSSAO_Vox_ConeTracing->getSphereInfo()->radDistParms.sphereOverlap = overlap;
        m_kernelSSAO_Vox_ConeTracing->reloadShaderInput();
      }else
      {
        float radius = m_kernelSSAO_Vox_ConeTracing->getSphereInfo()->radProgParms.sphereRadiusParm;
        radius = max((radius - .01f),0.0f);
        m_kernelSSAO_Vox_ConeTracing->getSphereInfo()->radProgParms.sphereRadiusParm = radius;
        m_kernelSSAO_Vox_ConeTracing->reloadShaderInput();
      }
      break;
    }
    break;


  case ')':
    switch(m_renderMode)
    {
    default:
      break;
    case SSAO_Vox_ConeTracing:
      if(m_SSAO_cone_infoMethod != "InitDist")
      {
        float overlap = m_kernelSSAO_Vox_ConeTracing->getSphereInfo()->radDistParms.sphereOverlap;
        overlap = min((overlap*1.25f),1.0f);
        m_kernelSSAO_Vox_ConeTracing->getSphereInfo()->radDistParms.sphereOverlap = overlap;
        m_kernelSSAO_Vox_ConeTracing->reloadShaderInput();
      }else
      {
        float center = m_kernelSSAO_Vox_ConeTracing->getSphereInfo()->radProgParms.sphereCenterParm;
        center = min((center + .1f),10.0f);
        m_kernelSSAO_Vox_ConeTracing->getSphereInfo()->radProgParms.sphereCenterParm = center;
        m_kernelSSAO_Vox_ConeTracing->reloadShaderInput();
      }
      break;
    }
    break;
  case '0':
    switch(m_renderMode)
    {
    default:
      break;
    case SSAO_Vox_ConeTracing:
      if(m_SSAO_cone_infoMethod != "InitDist")
      {
        float overlap = m_kernelSSAO_Vox_ConeTracing->getSphereInfo()->radDistParms.sphereOverlap;
        overlap = min((overlap + .01f),1.0f);
        m_kernelSSAO_Vox_ConeTracing->getSphereInfo()->radDistParms.sphereOverlap = overlap;
        m_kernelSSAO_Vox_ConeTracing->reloadShaderInput();
      }else
      {
        float radius = m_kernelSSAO_Vox_ConeTracing->getSphereInfo()->radProgParms.sphereRadiusParm;
        radius = min((radius + .01f),5.0f);
        m_kernelSSAO_Vox_ConeTracing->getSphereInfo()->radProgParms.sphereRadiusParm = radius;
        m_kernelSSAO_Vox_ConeTracing->reloadShaderInput();
      }
      break;
    }
    break;

  case 'P':
  case 'p':
    switch(m_renderMode)
    {
    default:
      break;
    case SSAO_Vox_ConeTracing:
      m_kernelSSAO_Vox_ConeTracing->getSphereInfo()->nextCalcMethod();
      m_kernelSSAO_Vox_ConeTracing->reloadShaderInput();
      break;
    }
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
    m_renderMode =  SSAO_SphereApproximation;
    if(modifier == GLUT_ACTIVE_SHIFT)
    {
      m_SSAO_rfarPercent = 30.0f;
      m_SSAO_contrast = 20.0;

      m_SSAO_pixelmaskSize = .8;
      m_SSAO_offsetSize = 5.0;
    }else 
      m_camHandler = m_kernelsCamHandleres[(m_debugrender_on?Debug:0) + m_renderMode];
    break;

  case 4: //F4
    m_renderMode = SSAO_HorizonSplit;
    m_camHandler = m_kernelsCamHandleres[(m_debugrender_on?Debug:0) + m_renderMode];
    break;
  
  case 5: //F5
    m_updateVoxelgrid = true;
    m_renderMode = Voxelization;
    m_camHandler = m_kernelsCamHandleres[m_renderMode];
    break;

  case 6: //F6
    m_renderMode = SSAO_Vox_RayMarch;

    if(modifier == GLUT_ACTIVE_SHIFT)
    {
      m_SSAO_rfarPercent = .06f;//.5f;
      m_SSAO_contrast = 2.14f;//1.0;
    }else 
      m_camHandler = m_kernelsCamHandleres[(m_debugrender_on?Debug:0) + m_renderMode];
    break;

  case 7: //F7
    m_renderMode = SSAO_Vox_TanSphereVolume;
    
    if(modifier == GLUT_ACTIVE_SHIFT)
    {
      m_SSAO_rfarPercent = .03f;
      m_SSAO_contrast = 1.0;
    }else 
      m_camHandler = m_kernelsCamHandleres[(m_debugrender_on?Debug:0) + m_renderMode];
    break;

  case 8: //F8
    m_renderMode = SSAO_Vox_ConeTracing;

    if(modifier == GLUT_ACTIVE_SHIFT)
    {
      m_SSAO_rfarPercent = .1f;
      m_SSAO_contrast = 1.28;

      m_kernelSSAO_Vox_ConeTracing->setNumCones(6);
      m_kernelSSAO_Vox_ConeTracing->setNumSpheresByCone(3);
      m_kernelSSAO_Vox_ConeTracing->setNumSphereSamplers(3);
      m_kernelSSAO_Vox_ConeTracing->setConeRevolutionAngle(DEG_TO_RAD(30));
    }else
      m_camHandler = m_kernelsCamHandleres[(m_debugrender_on?Debug:0) + m_renderMode];

    m_kernelSSAO_Vox_ConeTracing->setRfarPercent(m_SSAO_rfarPercent);
    m_kernelSSAO_Vox_ConeTracing->setContrast(m_SSAO_contrast);
    break;

  case 10: //F10
    m_wireframe_on = !m_wireframe_on;

    if(m_wireframe_on)
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    break;

  case 11: //F11
    m_renderMode = NoShader;
    m_camHandler = m_kernelsCamHandleres[(m_debugrender_on?Debug:0) + m_renderMode];
    break;

  case GLUT_KEY_PAGE_UP: //PgUp
    if(modifier == GLUT_ACTIVE_SHIFT)
      m_SSAO_rfarPercent *= 1.1f;
    else 
      m_SSAO_rfarPercent += .01f;
    
    if(m_renderMode == SSAO_Vox_ConeTracing)
      m_kernelSSAO_Vox_ConeTracing->setRfarPercent(m_SSAO_rfarPercent);
    break;

  case GLUT_KEY_PAGE_DOWN: //PgDown
    if(modifier == GLUT_ACTIVE_SHIFT)
      m_SSAO_rfarPercent = max(m_SSAO_rfarPercent*.9f, .000001f);
    else 
      m_SSAO_rfarPercent = max(m_SSAO_rfarPercent - .01f, .0000001f);
    
    if(m_renderMode == SSAO_Vox_ConeTracing)
      m_kernelSSAO_Vox_ConeTracing->setRfarPercent(m_SSAO_rfarPercent);
   
    break;

  case GLUT_KEY_HOME: //Home
    if(modifier == GLUT_ACTIVE_SHIFT)
      m_SSAO_contrast *= 1.1f;
    else 
      m_SSAO_contrast += .05;
    
    if(m_renderMode == SSAO_Vox_ConeTracing)
      m_kernelSSAO_Vox_ConeTracing->setContrast(m_SSAO_contrast);
    break;

  case GLUT_KEY_END: //END
    if(modifier == GLUT_ACTIVE_SHIFT)
      m_SSAO_contrast= max(m_SSAO_contrast*.9f, .01f);
    else 
      m_SSAO_contrast = max(m_SSAO_contrast - .05f, .01f);

    if(m_renderMode == SSAO_Vox_ConeTracing)
      m_kernelSSAO_Vox_ConeTracing->setContrast(m_SSAO_contrast);
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

void App::loadArgs(int argc, char *argv[])
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
      else cout << "Argument passed to "<< arg << " is invalid!" <<endl;
    }
  }
}
void App::loadSceneParameters()
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

void App::loadScene()
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

  int numMeshes = m_rtScene->getNumMeshes() + m_rtScene->getNumAnimatedMeshes();
  if(numMeshes == 0)
    cout << "No Mesh Loaded!!" <<endl;

  cout << "==================================="<<endl;
  cout << "Scene Number of Vertices: " << m_rtScene->getNumVertices() <<endl;
  cout << "Scene Number of Triangles: " << m_rtScene->getNumElements()<<endl;
  cout << "==================================="<<endl;
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


  m_kernelCombine = new KernelCombine((char*)m_shaderPath.c_str(), m_appWidth, m_appHeight
    ,m_kernelColor->getTexIdColor()
    );

  m_kernelVoxDepth = new KernelVoxDepth((char*)m_shaderPath.c_str(), m_appWidth, m_appHeight);

  m_kernelVoxelization = new KernelVoxelization((char*)m_shaderPath.c_str(), m_appWidth, m_appHeight 
    ,1 
    ,m_kernelVoxDepth->getTexIdEyePos()
    );


  m_kernelSSAO_SphereApproximation = new KernelSSAO_SphereApproximation((char*)m_shaderPath.c_str(), m_appWidth, m_appHeight
    ,m_kernelDeferred_Peeling->getTexIdPosition(0)
    ,m_kernelDeferred_Peeling->getTexIdNormal(0)
    ,m_kernelDeferred_Peeling->getTexIdNormal(1)
    ,m_kernelDeferred_Peeling->getTexIdNormal(2)
    );


  m_kernelSSAO_HorizonSplit = new KernelSSAO_HorizonSplit((char*)m_shaderPath.c_str(), m_appWidth, m_appHeight
    ,m_kernelDeferred_Peeling->getTexIdNormal(0)
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

  m_kernelSSAO_Vox_ConeTracing->setJitterEnabled(m_SSAO_jitter);
  if(m_SSAO_cone_infoMethod != "InitDist")
  {
    m_kernelSSAO_Vox_ConeTracing->getSphereInfo()->nextCalcMethod();
    m_kernelSSAO_Vox_ConeTracing->getSphereInfo()->radProgParms.sphereCenterParm = m_SSAO_cone_infoCenterParm;
    m_kernelSSAO_Vox_ConeTracing->getSphereInfo()->radProgParms.sphereRadiusParm = m_SSAO_cone_infoRadiusParm;
  }else m_kernelSSAO_Vox_ConeTracing->getSphereInfo()->radDistParms.sphereOverlap = m_SSAO_cone_infoSphereOverlap;
  
  m_kernelSSAO_Vox_ConeTracing->setConeRevolutionAngle(m_SSAO_cone_angle);
  m_kernelSSAO_Vox_ConeTracing->setNumCones(m_SSAO_cone_numCones);
  m_kernelSSAO_Vox_ConeTracing->setNumSpheresByCone(m_SSAO_cone_numSpheres);
  m_kernelSSAO_Vox_ConeTracing->setNumSphereSamplers(m_SSAO_cone_numSpamplers);
  m_kernelSSAO_Vox_ConeTracing->setRfarPercent(m_SSAO_rfarPercent);
  m_kernelSSAO_Vox_ConeTracing->setContrast(m_SSAO_contrast);

  GLfloat *lDif, *lPos;
  lDif = m_rtScene->getLightAt(0)->getLightStruct()->diffuse;
  lPos = m_rtScene->getLightAt(0)->getLightStruct()->pos;
  m_kernelDeferredLighting = new KernelDeferredLighting((char*)m_shaderPath.c_str(), m_appWidth, m_appHeight
    ,m_kernelVoxDepth->getTexIdNormalDepth()
    ,m_kernelVoxDepth->getTexIdEyePos()
    ,m_kernelSSAO_Vox_ConeTracing->getOutputTexture(0)
    ,Vector3(lPos[0], lPos[1], lPos[2])
    ,Vector3(lDif[0], lDif[1], lDif[2])
    );

  m_kernelBlur = new KernelBlur((char*)m_shaderPath.c_str(), m_appWidth, m_appHeight
    ,m_kernelSSAO_SphereApproximation->getColorTexId()
    );


  m_occlusionQuery = new GLOcclusionQuery();
  m_occlusionQuery->init();
}

void App::loadCameras()
{
  Vector3 bbSize = m_rtScene->getSceneBoundingBoxSize();
  float bbMaxSize = max(bbSize.x, max(bbSize.y, bbSize.z));

  m_fov = m_rtScene->getCamera()->getFovy();

  SphereGLCameraHandler *cam3D = new SphereGLCameraHandler(10.f, 0.f, 90.0f, 2.0f);

  SphereGLCameraHandler *voxDebug = new SphereGLCameraHandler(10.f, 0.f, 90.0f, 5.f);
  SphereGLCameraHandler *samplersDebug = new SphereGLCameraHandler(10.f, 0.f, 90.0f, 5.f);

  cam3D->setViewBoundingBox(m_rtScene->getSceneBoundingBoxMin(), m_rtScene->getSceneBoundingBoxMax(),  m_fov);
  voxDebug->setViewBoundingBox(m_kernelVoxelization->getVoxBBMin(), m_kernelVoxelization->getVoxBBMax(),  m_fov);
  samplersDebug->setViewBoundingBox(Vector3(-1.5f,-.5f,-1.5f), Vector3(1.5f,1.5f,1.5f),  m_fov);
  
  m_kernelsCamHandleres.push_back(cam3D); //NoShader
  m_kernelsCamHandleres.push_back(cam3D); //SSAO_SphereApproximation
  m_kernelsCamHandleres.push_back(cam3D); //SSAO_HorizonSplit
  m_kernelsCamHandleres.push_back(cam3D); //SSAO_Vox_RayMarch
  m_kernelsCamHandleres.push_back(cam3D); //SSAO_Vox_TanSphereVolume
  m_kernelsCamHandleres.push_back(cam3D); //SSAO_Vox_ConeTracing
  m_kernelsCamHandleres.push_back(cam3D); //Voxelization
  //Debug
  m_kernelsCamHandleres.push_back(cam3D); //NoShader
  m_kernelsCamHandleres.push_back(cam3D); //SSAO_SphereApproximation
  m_kernelsCamHandleres.push_back(cam3D); //SSAO_HorizonSplit
  m_kernelsCamHandleres.push_back(samplersDebug); //SSAO_Vox_RayMarch
  m_kernelsCamHandleres.push_back(samplersDebug); //SSAO_Vox_TanSphereVolume
  m_kernelsCamHandleres.push_back(samplersDebug); //SSAO_Vox_ConeTracing
  m_kernelsCamHandleres.push_back(voxDebug); //Voxelization

  for(int i = 1; i < m_rtScene->getNumCameras(); ++i)
  {
    ScCamera* scCam = m_rtScene->getCameraAt(i);
    GLCameraHandler *custom = new GLCameraHandler(scCam->getPos(), scCam->getAt(), scCam->getUp());
    m_kernelsCamHandleres.push_back(custom);
  }

  m_camHandler = cam3D;

  SphereGLCameraHandler* p = dynamic_cast<SphereGLCameraHandler*>(m_camHandler);
  if(p)
  {
    m_nearPlane = p->getSphereRadius()*.05f; 
    m_farPlane = p->getSphereRadius() + bbMaxSize * 1.5f;

    //m_nearPlane = p->getSphereRadius()*.85; 
    //m_farPlane =  p->getSphereRadius()*1.15; 

    //m_nearPlane = p->getSphereRadius()*1.0; 
    //m_farPlane =  p->getSphereRadius()*1.1; 

    //m_nearPlane = p->getSphereRadius()*1.0f; //.1
    //m_farPlane = p->getSphereRadius()*.8 + bbMaxSize * .01f; //1.5

    //m_nearPlane = p->getSphereRadius();
    //m_farPlane = p->getSphereRadius() + bbMaxSize;
  }else
  {
    m_nearPlane = .5;
    m_farPlane  = 10000;
  }


  GLLight *minerLight = m_camHandler->getMinerLight();
  minerLight->setAmbientColor(Color(0.0, 0.0, 0.0));
  minerLight->setDiffuseColor(Color(0.8, 0.8, 0.8));
  minerLight->setSpecularColor(Color(1.0, 1.0, 1.0));
  minerLight->setPosition(Vector3(0, 100, 0));
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

void App::voxelize()
{
  if(m_orthographicProjection_on)
  {
    glMatrixMode (GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity ();

    Vector3 size = m_rtScene->getSceneBoundingBoxSize();
    float orthoSize = max(max(size.x, size.y), size.z);
    glOrtho(-orthoSize, orthoSize, -orthoSize*m_appHeight/m_appWidth, orthoSize*m_appHeight/m_appWidth, m_nearPlane, m_farPlane);
    glMatrixMode (GL_MODELVIEW);
    glPushMatrix();
  }
  //glPushAttrib(GL_POLYGON_BIT);
  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //GL_POLYGON_BIT
  m_kernelVoxDepth->setActive(true);
  glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
  glEnable(GL_CULL_FACE);
  drawScene();
  m_kernelVoxDepth->setActive(false);

  m_voxProjectionMatrix->readGLProjection();

  m_kernelVoxelization->setActive(true);
  glDisable(GL_CULL_FACE);
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
}
void App::renderGUI()
{
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  m_fontRender->setSize(GLFont::Medium);
  m_fontRender->initText(m_appWidth, m_appHeight);

  char a[100];
  int yRight_i = 0;
  int yLeft_i = 0;
  float xTop = .65;
  float yRight = .03;
  
  ///Top Right//////////////////////////////
  //                                      //
  //////////////////////////////////////////
  sprintf(a,"(F1) %s Menu", m_menu_on?"Close":"Open");
  m_fontRender->print(m_appWidth*xTop*.95f, m_appHeight*yRight + 25*yRight_i++,a, Color(0., 0., 0.));

  if(m_menu_on)
  {
    sprintf(a,"(O) Proj: %s", m_orthographicProjection_on? "Orthographic":"Perspective");
    m_fontRender->print(m_appWidth*xTop, m_appHeight*yRight + 25*yRight_i++,a, Color(0., 0., 0.));

    sprintf(a,"(PgUp/PgDn) Rfar: %.5f ", m_SSAO_rfarPercent);
    m_fontRender->print(m_appWidth*xTop, m_appHeight*yRight + 25*yRight_i++,a, Color(0., 0., 0.));

    sprintf(a,"(Home/End) Contrast: %.2f ", m_SSAO_contrast);
    m_fontRender->print(m_appWidth*xTop, m_appHeight*yRight + 25*yRight_i++,a, Color(0., 0., 0.));
  }

  switch(m_renderMode)
  {
    case SSAO_SphereApproximation:
    case SSAO_HorizonSplit:
      if(m_menu_on)
      {
        sprintf(a,"(L) Lights %s", m_lights_on? "On":"Off");
        m_fontRender->print(m_appWidth*xTop, m_appHeight*yRight + 25*yRight_i++,a, Color(0., 0., 0.));

        sprintf(a,"(M) Mine Light: %s", m_minerLight_on? "On":"Off");
        m_fontRender->print(m_appWidth*xTop, m_appHeight*yRight + 25*yRight_i++,a, Color(0., 0., 0.));

        sprintf(a,"(B) Blur %s", m_blurr_on? "On":"Off");
        m_fontRender->print(m_appWidth*xTop, m_appHeight*yRight + 25*yRight_i++,a, Color(0., 0., 0.));

        sprintf(a,"(+/-)%d Num Peelings", m_SSAO_numPeelings);
        m_fontRender->print(m_appWidth*xTop, m_appHeight*yRight + 25*yRight_i++,a, Color(0., 0., 0.));

        sprintf(a,"(I/U) Pixmask: %.3f ", m_SSAO_pixelmaskSize);
        m_fontRender->print(m_appWidth*xTop, m_appHeight*yRight + 25*yRight_i++,a, Color(0., 0., 0.));

        sprintf(a,"(K/J) Offsets_size: %.1f ", m_SSAO_offsetSize);
        m_fontRender->print(m_appWidth*xTop, m_appHeight*yRight + 25*yRight_i++,a, Color(0., 0., 0.));
      }
    default:
    case NoShader:
      if(m_menu_on)
      {
        sprintf(a,"(L) Lights %s", m_lights_on? "On":"Off");
        m_fontRender->print(m_appWidth*xTop, m_appHeight*yRight + 25*yRight_i++,a, Color(0., 0., 0.));

        sprintf(a,"(M) Mine Light: %s", m_minerLight_on? "On":"Off");
        m_fontRender->print(m_appWidth*xTop, m_appHeight*yRight + 25*yRight_i++,a, Color(0., 0., 0.));

        sprintf(a,"(B) Blur %s", m_blurr_on? "On":"Off");
        m_fontRender->print(m_appWidth*xTop, m_appHeight*yRight + 25*yRight_i++,a, Color(0., 0., 0.));
      }
      sprintf(a,"%s", STR(NoShader));
      break;

    case SSAO_Vox_RayMarch:
      break;
    case SSAO_Vox_TanSphereVolume:
      break;
    case SSAO_Vox_ConeTracing:
      if(m_menu_on)
      {
        sprintf(a,"(X) Diffuse: %s", m_diffuseEnabled? "On":"Off");
        m_fontRender->print(m_appWidth*xTop, m_appHeight*yRight + 25*yRight_i++,a, Color(0., 0., 0.));

        sprintf(a,"(Z) AO: %s", m_aoEnabled? "On":"Off");
        m_fontRender->print(m_appWidth*xTop, m_appHeight*yRight + 25*yRight_i++,a, Color(0., 0., 0.));

        sprintf(a,"(S) OcclusionQuery: %s", m_occlusionQueryEnabled? "On":"Off");
        m_fontRender->print(m_appWidth*xTop, m_appHeight*yRight + 25*yRight_i++,a, Color(0., 0., 0.));

        sprintf(a,"(N) Jitter: %s", m_SSAO_jitter? "On":"Off");
        m_fontRender->print(m_appWidth*xTop, m_appHeight*yRight + 25*yRight_i++,a, Color(0., 0., 0.));

        sprintf(a,"(I/U) N Cones: %d ", m_kernelSSAO_Vox_ConeTracing->getNumCones());
        m_fontRender->print(m_appWidth*xTop, m_appHeight*yRight + 25*yRight_i++,a, Color(0., 0., 0.));

        sprintf(a,"(K/J)N Spheres: %d ", m_kernelSSAO_Vox_ConeTracing->getNumSpheresByCone());
        m_fontRender->print(m_appWidth*xTop, m_appHeight*yRight + 25*yRight_i++,a, Color(0., 0., 0.));

        sprintf(a,"(G/H) N Spheres Samplers: %d ", m_kernelSSAO_Vox_ConeTracing->getNumberSphereSamplers());
        m_fontRender->print(m_appWidth*xTop, m_appHeight*yRight + 25*yRight_i++,a, Color(0., 0., 0.));

        sprintf(a,"(7/8) Cone Angle: %.2f ", RAD_TO_DEG(m_kernelSSAO_Vox_ConeTracing->getConeRevolutionAngle()));
        m_fontRender->print(m_appWidth*xTop, m_appHeight*yRight + 25*yRight_i++,a, Color(0., 0., 0.));

        sprintf(a,"(P) SphereInfo Method: %d ", (int)m_kernelSSAO_Vox_ConeTracing->getSphereInfo()->currCalcMethod);
        m_fontRender->print(m_appWidth*xTop, m_appHeight*yRight + 25*yRight_i++,a, Color(0., 0., 0.));

        if(m_SSAO_cone_infoMethod != "InitDist")
        {
          sprintf(a,"(9/0) SphereOverlap: %.2f ", m_kernelSSAO_Vox_ConeTracing->getSphereInfo()->radDistParms.sphereOverlap);
          m_fontRender->print(m_appWidth*xTop, m_appHeight*yRight + 25*yRight_i++,a, Color(0., 0., 0.));
        }else
        {
          sprintf(a,"('(' / ')') SphereCenter: %.2f ", m_kernelSSAO_Vox_ConeTracing->getSphereInfo()->radProgParms.sphereCenterParm);
          m_fontRender->print(m_appWidth*xTop, m_appHeight*yRight + 25*yRight_i++,a, Color(0., 0., 0.));

          sprintf(a,"(9/0) SphereRadius: %.2f ", m_kernelSSAO_Vox_ConeTracing->getSphereInfo()->radProgParms.sphereRadiusParm);
          m_fontRender->print(m_appWidth*xTop, m_appHeight*yRight + 25*yRight_i++,a, Color(0., 0., 0.));
        }
      }
      if(m_occlusionQueryEnabled)
      {
        sprintf(a,"%.3fk Frags", (float)m_occlusionQuery->getNumberOfFragments()/1000);
        m_fontRender->print(m_appWidth*.85,m_appHeight*.85 + 25, a, Color(0., 0., 0.));
      }
      break;
  }

  ///Top Left///////////////////////////////
  //                                      //
  //////////////////////////////////////////
  m_fontRender->print(m_appWidth*.03,m_appHeight*.03 + 25*yLeft_i++, s_renderModeStr[m_renderMode].c_str(), Color(0., 0., 0.));

  if(m_wireframe_on)
  {
    sprintf(a,"(F10) WireFrame On");
    m_fontRender->print(m_appWidth*.03,m_appHeight*.03 + + 25*yLeft_i++, a, Color(0., 0., 0.));
  }

  if(m_debugrender_on)
  {
    sprintf(a,"(D) Render Debug");
    m_fontRender->print(m_appWidth*.03,m_appHeight*.03 + 25*yLeft_i++, a, Color(0., 0., 0.));
  }

  //////////////////////////////////////////
  //                                      //
  ////////////////////////////Bottom Right//
  sprintf(a,"%.2f FPS", m_frames->getFPS());
  m_fontRender->print(m_appWidth*.85,m_appHeight*.85 + 55,a, Color(0., 0., 0.));

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
    glOrtho(-orthoSize, orthoSize, -orthoSize*m_appHeight/m_appWidth, orthoSize*m_appHeight/m_appWidth, m_nearPlane, m_farPlane);

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
  //COLOR PASS
  m_kernelColor->setActive(true);
  glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
  glDisable(GL_CULL_FACE);
  drawScene();

  m_kernelColor->setActive(false);

  GLfloat projectionMatrix[16];
  glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);

  //DEPTH PEELING PASS
  //for(int i = 0; i < m_SSAO_numPeelings; ++i)
  int i = 0;
  {
    m_kernelDeferred_Peeling->step(i);
    m_kernelDeferred_Peeling->setActive(true);

    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    glDisable(GL_CULL_FACE);
    drawScene();

    m_kernelDeferred_Peeling->setActive(false);
  }

  //SSAO PASS
  m_kernelSSAO_SphereApproximation->step(projectionMatrix, m_SSAO_rfarPercent, m_SSAO_pixelmaskSize,m_SSAO_offsetSize, m_SSAO_contrast);

  //BLURR PASS
  if(m_blurr_on)
  {
    m_kernelBlur->setInputTexId(m_kernelSSAO_SphereApproximation->getColorTexId());
    m_kernelBlur->step(1);
  }

  //COMBINE PASS
  if(m_blurr_on)
    m_kernelCombine->step(m_kernelBlur->getBlurredTexId());
  else
    m_kernelCombine->step(m_kernelSSAO_SphereApproximation->getColorTexId());

  //RENDER RESULT
  m_kernelCombine->renderOutput(0);
  //m_kernelSSAO_SphereApproximation->renderOutput(0);
}

void App::renderSSAOHorizonSplit()
{
  //COLOR PASS
  m_kernelColor->setActive(true);
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    glDisable(GL_CULL_FACE);
    drawScene();
  m_kernelColor->setActive(false);

  GLfloat projectionMatrix[16];
  glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);

  //DEPTH PEELING PASS
  for(int i=0; i < m_SSAO_numPeelings; ++i)
  {
    m_kernelDeferred_Peeling->step(i);
    m_kernelDeferred_Peeling->setActive(true);

    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    glDisable(GL_CULL_FACE);
    drawScene();

    m_kernelDeferred_Peeling->setActive(false);
  }
  //SSAO PASS
  m_kernelSSAO_HorizonSplit->step(projectionMatrix, m_SSAO_rfarPercent, m_SSAO_pixelmaskSize,m_SSAO_offsetSize, m_SSAO_contrast);

  //BLURR PASS
  if(m_blurr_on)
  {
    m_kernelBlur->setInputTexId(m_kernelSSAO_HorizonSplit->getColorTexId());
    m_kernelBlur->step(1);
  }

  //COMBINE PASS
  if(m_blurr_on)
    m_kernelCombine->step(m_kernelBlur->getBlurredTexId());
  else
    m_kernelCombine->step(m_kernelSSAO_HorizonSplit->getColorTexId());

  //RENDER RESULT
  m_kernelCombine->renderOutput(0);
}
void App::renderVoxelization()
{
  if(m_updateVoxelgrid)
  {
    voxelize();

    m_updateVoxelgrid = false;
    m_kernelsCamHandleres[Debug + Voxelization]->setViewBoundingBox(m_kernelVoxelization->getVoxBBMin(), m_kernelVoxelization->getVoxBBMax(),  m_fov);
    m_camHandler = m_kernelsCamHandleres[Debug + Voxelization];
  }else
  {
    Vector3 bb_size = m_kernelVoxelization->getVoxBBMax() - m_kernelVoxelization->getVoxBBMin();
    float maxsize = max(max(bb_size.x, bb_size.y), bb_size.z);

    glMatrixMode (GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity ();
    gluPerspective(m_fov, (GLfloat)m_appWidth/(GLfloat)m_appHeight, maxsize*.1f, maxsize*10.0f);
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
}
void App::renderSSAOVoxRayMarch()
{
  voxelize();

  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  m_kernelSSAO_Vox_RayMarch->step(m_voxProjectionMatrix, m_SSAO_rfarPercent, m_SSAO_contrast);
  m_kernelSSAO_Vox_RayMarch->renderOutput(KernelSSAO_Vox_RayMarch::SSAO);

  //GLTextureObject texObj = GLTextureObject(m_kernelSSAO_Voxelization->getOutputTexture(1));
  //GLuint* i = texObj.read2DTextureUIntData();
  //GLTextureObject t2 = GLTextureObject(m_kernelSSAO_Voxelization->getOutputTexture(0));
  //GLfloat* f = t2.read2DTextureFloatData();
  glPopAttrib();
}


void App::renderSSAOVoxTanSphereVolume()
{
  voxelize();

  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  m_kernelSSAO_Vox_TanSphereVolume->step(m_voxProjectionMatrix, m_SSAO_rfarPercent, m_SSAO_contrast);
  m_kernelSSAO_Vox_TanSphereVolume->renderOutput(KernelSSAO_Vox_RayMarch::SSAO);

  //GLTextureObject texObj = GLTextureObject(m_kernelSSAO_Voxelization_Volume->getOutputTexture(1));
  //GLuint* i = texObj.read2DTextureUIntData();
  //GLTextureObject t2 = GLTextureObject(m_kernelSSAO_Voxelization_Volume->getOutputTexture(0));
  //GLfloat* f = t2.read2DTextureFloatData();

  glPopAttrib();
}


void App::renderSSAOVoxConeTracing()
{
  voxelize();
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  m_kernelSSAO_Vox_ConeTracing->step(m_voxProjectionMatrix);

  //BLURR PASS
  if(m_blurr_on)
  {
    m_kernelBlur->setInputTexId(m_kernelSSAO_Vox_ConeTracing->getTexIdSSAO());
    m_kernelBlur->step(1);
    m_kernelBlur->renderOutput(0);
  }else
  {
    m_rtScene->setMaterialActive(true, 1);

    if(m_occlusionQueryEnabled)
    {
      m_occlusionQuery->begin();
      m_kernelDeferredLighting->stepShaderOnly((int)m_aoEnabled, (int)m_diffuseEnabled, Vector3(.2, .2, .2));
      m_occlusionQuery->end();
    }else 
      m_kernelDeferredLighting->step((int)m_aoEnabled, (int)m_diffuseEnabled, Vector3(.35, .35, .35));

    m_rtScene->setMaterialActive(false, 1);

    if(!m_occlusionQueryEnabled)
      m_kernelDeferredLighting->renderOutput(0);

    
    //m_kernelSSAO_Vox_ConeTracing->renderOutput(KernelSSAO_Vox_ConeTracing::SSAO);
  }


  //GLTextureObject texObj = GLTextureObject(m_kernelVoxelization->getTexIdGrid0());
  //GLuint* i = texObj.read2DTextureUIntData();
  //GLTextureObject t2 = GLTextureObject(m_kernelSSAO_Vox_ConeTracing->getOutputTexture(0));
  //GLfloat* f = t2.read2DTextureFloatData();
  //GLuint* i = (GLuint*)t2.read2DTextureFloatData();

  glPopAttrib();
}



