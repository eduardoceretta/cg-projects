#ifndef _MAIN_H_
#define _MAIN_H_

#include "GraphBasis/GraphBasisDefines.h"


#define APP_NAME "DeferredShading"
#define APP_INITIAL_WIDTH 800
#define APP_INITIAL_HEIGHT 600

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