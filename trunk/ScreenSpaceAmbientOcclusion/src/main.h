/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  May 2011
 *
 *  Application's Main code.
 *  Create the application and bind the GLUT callbacks.
 */
#ifndef _MAIN_H_
#define _MAIN_H_

#include "MathUtils\MathDefines.h"

/**
 * Main. Initializes the application and start GLUT Loop
 */
int main(int argc, char *argv[]);

/**
 * Initializes the GLUT callbacks
 */
void setGlutCallbacks();


/************************************************************************/
/* GLUT CALLBACKS                                                       */
/************************************************************************/

/**
 * Called at window reshape event
 */
void reshape(int w, int h);

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
void mouseClick(int button, int state, int x, int y);

/**
 * Called at mouse movement event
 */
void mouseMove(int x, int y);

/**
 * Called at screen drawing event and when idle
 */
void display();



#endif