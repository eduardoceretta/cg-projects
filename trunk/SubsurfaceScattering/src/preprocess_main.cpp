#include <iostream>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <cmath>

#include "GraphBasis/GLFont.h"
#include "GraphBasis/Frames.h"

#include "Objects\Mesh.h"
#include "PreProcess\PreProcessor.h"

#include "main.h"


int main(int argc, char *argv[]){
  Mesh m;
  char c[] = "0 0 0 0 1 1 1 ./resources/Models/dragon_high.msb\n";
  m.readFromStr(c);
  PreProcessor preProcessor(m.getVbo());
}

