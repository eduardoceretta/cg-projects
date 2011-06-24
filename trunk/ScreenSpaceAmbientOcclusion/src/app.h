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
#ifndef _APP_H_
#define _APP_H_

#include <string>
#include <map>

/**
 * Application Initial Definitions
 */
#define APP_NAME "ScreenSpaceAmbientOcclusion"

#define APP_INITIAL_WIDTH 512
#define APP_INITIAL_HEIGHT 512
#define APP_FOV 60.0f
#define APP_NEAR 5.0f
#define APP_FAR 1000.0f
#define APP_DEFAULT_SCENE_PATH "./resources/Scenes/scene.rt4"
#define APP_DEFAULT_SHADER_PATH "./resources/Shaders/"

/**
 * Forward Class Declaration
 */
class ScScene;
class P3bMeshFile;

class SphereGLCameraHandler;
class GLFont;
class Frames;

class KernelColor;
class KernelDeferred_Peeling;
class KernelSSAO;
class KernelBlur;
class KernelCombine;

class GLTextureObject;

using namespace std;

class App 
{
  /**
   * Command line Arguments
   */
  int m_argc;
  char **m_argv;

  map<string, string*> m_acceptedArgsString;
  map<string, int*> m_acceptedArgsInt;
  map<string, float*> m_acceptedArgsFloat;

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

  /**
   * Render Objects
   */
  SphereGLCameraHandler *m_camHandler;
  GLFont *m_fontRender;
  Frames *m_frames;
  float m_fps;

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
  KernelSSAO* m_kernelSSAO;
  KernelBlur* m_kernelBlur;
  KernelCombine* m_kernelCombine;

  /**
   * Interface control
   */
  bool m_menu_on;
  bool m_lights_on;
  bool m_minerLight_on;
  bool m_wireframe_on;
  bool m_shader_on;
  bool m_shader_active;

  /**
   * Algorithm Parameters
   */
  float m_rfar;
  float m_pixelmaskSize;
  float m_offsetSize;
  float m_intensity;
  int m_numPeelings;
  bool m_blurr_on;

  /**
   * Debug
   */
  GLenum e;
  GLTextureObject *m_texDebug;

public:
  App();
  ~App();
  
  void initGL(int *argc, char *argv[]);
  
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

  /**
   * Manages a simple keyboard event
   */
  void listenKeyboard(int key);

  /**
   * Manages a special keyboard event
   */
  void listenKeyboardSpecial( int key );

  /**
   * Manages a mouse movement event
   */
  void listenMouseMove(int x, int y);

  /**
   * Manages a mouse button press event
   */
  void listenMouseClick(int button, int state, int x, int y);

private:
  /**
   * Process the application arguments
   */
  void processArgs();

  /**
   * Load and Initializes the Scene to be rendered
   */
  void loadScene();
  
  /**
   * Initializes the kernels that will process the scene
   */
  void loadKernels();

  /**
   * Draw the Scene
   */
  void drawScene();
 
  /**
   * Draw the Graphical User Interface
   */
  void renderGUI();


};

#endif