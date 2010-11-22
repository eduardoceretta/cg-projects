#include <iostream>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <cmath>

#include "GraphBasis/GLFont.h"
#include "GraphBasis/Frames.h"

#include "MeshLoaders\MeshLoader.h"
#include "PreProcess\PreProcessor.h"

#include "main.h"

#define CONFIG_FILENAME "./resources/config.conf"
struct parms 
{
  string path;
  string fileName;
  string fileType;
  string outFileType;
  float ca;
  float cs;
  float g;
  float n1;
  float n2;
  float neighborDistance;
  Vector3 lightDir;
};
struct parms  readFile(string configFileName);

int main(int argc, char *argv[]){
  struct parms p = readFile(CONFIG_FILENAME);
  
  MeshLoader m;
  string path = p.path;
  string fileName = p.fileName;
  m.readFile(path+fileName+p.fileType);
  
  PreProcessor preProcessor(m.getVertexes(), m.getNormals(), m.getNumVertices(), m.getIndexes(), m.getNumTriangles());
  preProcessor.setLightDirection(p.lightDir);
  preProcessor.setCa(p.ca);
  preProcessor.setCs(p.cs);
  preProcessor.setG(p.g);
  preProcessor.setN(REFRACTIVE_INDEX_N1, REFRACTIVE_INDEX_N2);
  preProcessor.setNeighborDistance(p.neighborDistance);// So far so good .01
  preProcessor.calc();
  preProcessor.writeTextures(path+fileName+p.outFileType);
}

struct parms  readFile(string configFileName)
{
  struct parms retParms;

  struct args 
  {
    string name;
    string type;
    void *adress;
  };
 struct args parameters[] = 
 {
   {"PATH",             "STRING", (void*)&retParms.path}
  ,{"FILENAME",         "STRING", (void*)&retParms.fileName}
  ,{"FILETYPE",         "STRING", (void*)&retParms.fileType}
  ,{"OUTPUT_FILETYPE",         "STRING", (void*)&retParms.outFileType}
  ,{"CA",               "FLOAT",  (void*)&retParms.ca}
  ,{"CS",               "FLOAT",  (void*)&retParms.cs}
  ,{"G",                "FLOAT",  (void*)&retParms.g}
  ,{"N1",               "FLOAT",  (void*)&retParms.n1}
  ,{"N2",               "FLOAT",  (void*)&retParms.n2}
  ,{"NEIGHBORDISTANCE", "FLOAT",  (void*)&retParms.neighborDistance}
  ,{"LIGHT_DIR", "VECTOR3",  (void*)&retParms.lightDir}
 };
  

 
  FILE *fp = fopen(configFileName.c_str(), "r");
  MyAssert("File Could Not be Found", fp);
  char buffer[1024];
  char parmName[100];
  while(!feof(fp))
  {
    fscanf(fp, "%[^\n]s", buffer);
    sscanf(buffer, "%s = %[^\n]s",parmName, buffer);
    for(int i=0; i < sizeof(parameters)/sizeof(string); ++i)
      if(parameters[i].name==parmName)
      {
        if(parameters[i].type=="STRING")
          *((string*)parameters[i].adress) = string(buffer) ;
        else if(parameters[i].type=="FLOAT")
          sscanf(buffer, "%f", parameters[i].adress);
        else if(parameters[i].type=="VECTOR3")
          sscanf(buffer, "%f %f %f", parameters[i].adress, ((float *)parameters[i].adress)+1, ((float *)parameters[i].adress)+2);
        break;
      }
    fgetc(fp);
  }
  fclose(fp);
  return retParms;
}

