#ifndef _MAIN_H_
#define _MAIN_H_

#include "GraphBasis/GraphBasisDefines.h"
#include <assert.h>

#define APP_NAME "ScreenSpaceAmbientOcclusion"

#define APP_INITIAL_WIDTH 512
#define APP_INITIAL_HEIGHT 512
#define APP_FOV 60.0f
#define APP_NEAR 5.0f
#define APP_FAR 1000.0f

#define P3DMODEL_NAME_BIN "D:/users/favera/My Dropbox/Data/Mestrado/Papers Tese/SSAO/SSAO/resources/Models/TecGraf/16metros_30graus.p3b"
#define P3DMODEL_NAME_LUA "D:/users/favera/My Dropbox/Data/Mestrado/Papers Tese/SSAO/SSAO/resources/Models/TecGraf/16metros_30graus.p3i"

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
void renderUIText();
void setupCamera();
void createScenes();
inline void render();

#endif