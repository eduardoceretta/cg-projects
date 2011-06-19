/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  May 2011
 *
 *  Application's Main code.
 *  Receive a Scene description file renders the scene with ambient occlusion.
 *
 *  Initializes the scene, process the geometry via diferent kernel
 *  passes and combine results to produce the final scene with the
 *  ambient occlusion.
 */
#ifndef _MAIN_H_
#define _MAIN_H_

#include "MathUtils\MathDefines.h"

/**
 * Application Initial Definitions
 */
#define APP_NAME "ScreenSpaceAmbientOcclusion"

#define APP_INITIAL_WIDTH 512
#define APP_INITIAL_HEIGHT 512
#define APP_FOV 60.0f
#define APP_NEAR 5.0f
#define APP_FAR 1000.0f
#define APP_DEFAULT_PATH "./resources/scenes/cavalo.rt4"

/**
 * Main. Initializes the application and start GLUT Loop
 */
int main(int argc, char *argv[]);

/**
 * Initializes the OpenGL context
 */
void initGL(int *argc, char *argv[]);

/**
 * Load and Initializes the Scene to be rendered
 */
void loadScene(int argc, char *argv[]);

/**
 * Initializes the kernels that will process the scene
 */
void createKernels();

/**
 * Draw the Graphical User Interface
 */
void renderGUI();

/**
 * Draw the scene
 */
void render();


/************************************************************************/
/* GLUT CALLBACKS                                                       */
/************************************************************************/

/**
 * Called at window reshape event
 */
void reshapeGL(int w, int h);

/**
 * Called at simple keyboard event
 */
void keyboard(unsigned char key, int x, int y);

/**
 * Called at special keyboard event
 */
void keyboardSpecial(int key, int x, int y);

/**
 * Called at mouse button press event
 */
void mouseButtons(int button, int state, int x, int y);

/**
 * Called at mouse movement event
 */
void mouseActive(int x, int y);

/**
 * Called at screen drawing event and when idle
 */
void display();



#endif