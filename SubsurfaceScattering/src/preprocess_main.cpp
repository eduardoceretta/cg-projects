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
  string path = "./resources/Models/";
  string fileName = "dragon_low";
  m.readFile(path+fileName+".msh");
  PreProcessor preProcessor(m.getVertexes(), m.getNormals(), m.getNumVertices(), m.getIndexes(), m.getNumTriangles());
  preProcessor.setCa(1.0);
  preProcessor.setCs(1.0);
  preProcessor.setG(.5);
  preProcessor.setN(REFRACTIVE_INDEX_N1, REFRACTIVE_INDEX_N2);
  preProcessor.setNeighborDistance(.01);// So far so good
  preProcessor.calc();
  preProcessor.writeTextures(path+fileName+".stex");
}

