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
#ifndef _TEST_APP_H_
#define _TEST_APP_H_

#include <string>
#include <vector>
#include <map>


/**
 * Application Initial Definitions
 */
#define APP_NAME "AmbientOcclusionTest"

#define APP_DEFAULT_WIDTH 512
#define APP_DEFAULT_HEIGHT 512
#define APP_DEFAULT_FOV 60.0f
#define APP_DEFAULT_NEAR 5.0f
#define APP_DEFAULT_FAR 1000.0f
#define APP_DEFAULT_SCENE_PATH "./resources/Scenes/scene.rt4"
#define APP_DEFAULT_SHADER_PATH "./resources/Shaders/"

#define APP_DEFAULT_SCREEN_TEST_PATH "./resources/Tests/ScreenShots/2/"
#define APP_DEFAULT_LOG_PATH "./resources/Tests/ScreenShots/2/"

/**
 * Forward Class Declaration
 */
class ScScene;
class P3bMeshFile;
class GLFont;

class GLCameraHandler;

class GLProjectionMatrix;

class KernelColor;
class KernelDeferred_Peeling;
class KernelBlur;
class KernelCombine;
class KernelVoxDepth;
class KernelVoxelization;
class KernelSSAO_SphereApproximation;
class KernelSSAO_Vox_RayMarch;
class KernelSSAO_Vox_ConeTracing;

class TimeTest;
class ScreenShotTest;

using namespace std;


class TestApp 
{
  map<string, string*> m_acceptedArgsString;
  map<string, int*> m_acceptedArgsInt;
  map<string, float*> m_acceptedArgsFloat;
  map<string, bool*> m_acceptedArgsBool;

  /**
   * App Resources Paths
   */
  string m_scenePath;
  string m_shaderPath;  

  /**
   * Application Parameters
   */
  int m_appWidth;
  int m_appHeight;
  float m_nearPlane;
  float m_farPlane;
  float m_fov;
  GLfloat m_clearColor[4];
  GLfloat m_ambientColor[4];

  /**
   * Render Objects
   */
  vector<GLCameraHandler*> m_customCamHandleres;
  GLFont *m_fontRender;

  /**
   * Scene Objects
   */
  ScScene* m_rtScene;
  P3bMeshFile* m_p3bMesh;

  /**
   * Kernel Objects
   */
  KernelColor* m_kernelColor;
  KernelDeferred_Peeling* m_kernelDeferred_Peeling;
  KernelBlur* m_kernelBlur;
  KernelCombine* m_kernelCombine;
  KernelVoxDepth* m_kernelVoxDepth;
  KernelVoxelization* m_kernelVoxelization;

  KernelSSAO_SphereApproximation* m_kernelSSAO_SphereApproximation;
  KernelSSAO_Vox_RayMarch* m_kernelSSAO_Vox_RayMarch;
  KernelSSAO_Vox_ConeTracing * m_kernelSSAO_Vox_ConeTracing;

  /**
   * Voxelization Algorithm Parameters
   */
  GLProjectionMatrix *m_voxProjectionMatrix;

  /**
   * Test Parameters
   */
  string m_logPath;
  string m_logFileName;

  TimeTest* m_timeTest;
  bool m_timeTestEnabled;

  ScreenShotTest* m_screenShotTest;
  string m_screenShotsPath;
  bool m_screenShotTestEnabled;
  int m_testCamIndex;


  /**
   * Algorithms Parameters
   */
  bool m_Sphere_enabled;
  float m_Sphere_pixelmaskSize;
  float m_Sphere_offsetSize;
  int m_Sphere_numPeelings;
  float m_Sphere_rfarPercent;
  float m_Sphere_contrast;

  bool m_RayMarch_enabled;
  float m_RayMarch_rfarPercent;
  float m_RayMarch_contrast;

  bool m_ConeTracing_enabled;
  bool m_ConeTracing_jitter;
  float m_ConeTracing_rfarPercent;
  float m_ConeTracing_contrast;
  float m_ConeTracing_coneAngle;
  int m_ConeTracing_numCones;
  int m_ConeTracing_numSpheres;
  int m_ConeTracing_numSpamplers;
  string m_ConeTracing_sphereInfoMethod;
  float m_ConeTracing_sphereInfoSphereOverlap;

  /**
   * Render Mode
   */
  enum RenderMode{NoShader = 0
                 ,Sphere
                 ,RayMarch
                 ,ConeTracing
                 ,Debug
                 ,CustomCameras = 2*Debug};

  static string s_renderModeStr[];

  RenderMode m_renderMode;

  vector<RenderMode> m_algorithms;

  /**
   * Debug
   */
  GLenum e;

public:
  TestApp();
  ~TestApp();
  
  void initGL(int *argc, char *argv[]);
  
  
  /**
   * Load scene and application parameters (win size, clear color...)
   */
  void loadParameters(int argc, char *argv[]);
  
  /**
   * Load and Initializes the Application Resources
   */
  void loadResources();

  /**
   * Draw the scene
   */
  void render();

  /**
   * Manages a window reshape event
   */
  void listenReshape(int w, int h);

private:
  /**
   * Process the application arguments
   */
  void loadArgs(int argc, char *argv[]);
  
  /**
   * Load and Initializes the Scene objects to be rendered
   */
  void loadScene();

  /**
   * Load and Initializes the testes to be executed
   */
  void loadTests();
  
  /**
   * Load and Initializes the Scene parameters (win size, clear color...)
   */
  void loadSceneParameters();
  
  /**
   * Initializes the kernels that will process the scene
   */
  void loadKernels();

  /**
   * Initializes the cameras that will display the scene
   */
  void loadCameras();

  /**
   * Draw the Scene
   */
  void drawScene();
 
  /************************************************************************/
  /* Static Functions called by the testers                               */
  /************************************************************************/

  /**
   * Create geometry buffers of the Scene
   */
  static void deferredShading(TestApp* );

  /**
   * Voxelize the Scene
   */
  static void voxelize(TestApp* );

  /**
   * Kernels
   */
  static void sphere(TestApp* );
  static void rayMarch(TestApp* );
  static void coneTracing(TestApp* );
  
};

#endif