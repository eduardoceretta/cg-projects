#ifndef _MAIN_H_
#define _MAIN_H_

#include <assert.h>
#include <iostream>
#include "GraphBasis/GraphBasisDefines.h"


#define APP_NAME "DeferredShading"
#define APP_INITIAL_WIDTH 800
#define APP_INITIAL_HEIGHT 600

#define REFRACTIVE_INDEX_N1 1.0f
#define REFRACTIVE_INDEX_N2 1.3f

#ifdef NDEBUG
  #define MyAssert(str, expression)     ((void)0)  
#else
  #define MyAssert(str, expression) if(!(expression)){std::cout<<(str)<<std::endl;assert(expression);}
#endif

extern int appWidth;
extern int appHeight;

int main(int argc, char *argv[]);

void init(int argc, char *argv[]);

void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void keyboardSpecial(int key, int x, int y);
void mouseButtons(int button, int state, int x, int y);
void mouseActive(int x, int y);
void display();

void renderAxis();

void createScenes();
inline void render();

#endif