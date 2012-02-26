/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Feb 2011
 *
 *  Test Application's Main code.
 *  Create the test application and bind the GLUT callbacks.
 */
#ifndef _TEST_MAIN_H_
#define _TEST_MAIN_H_

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
 * Called at screen drawing event and when idle
 */
void display();



#endif