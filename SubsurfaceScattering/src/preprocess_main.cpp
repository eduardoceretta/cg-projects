#include <iostream>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <cmath>

#include "GraphBasis/GLFont.h"
#include "GraphBasis/Frames.h"

#include "MeshLoaders\MeshLoader.h"
#include "PreProcess\PreProcessor.h"

#include "main.h"


int main(int argc, char *argv[]){
  MeshLoader m;
  m.readFile("./resources/Models/dragon_high.msh");
  PreProcessor preProcessor(m.getVertexes(), m.getNormals(), m.getNumVertices(), m.getIndexes(), m.getNumTriangles());
  
}

