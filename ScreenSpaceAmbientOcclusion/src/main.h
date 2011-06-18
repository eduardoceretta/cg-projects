#ifndef _MAIN_H_
#define _MAIN_H_

#include "MathUtils\MathDefines.h"
#include <assert.h>

#define APP_NAME "ScreenSpaceAmbientOcclusion"

#define APP_INITIAL_WIDTH 512
#define APP_INITIAL_HEIGHT 512
#define APP_FOV 60.0f
#define APP_NEAR 5.0f
#define APP_FAR 1000.0f



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
void renderUIText();
void setupCamera();
void createScenes();
inline void render();

#endif