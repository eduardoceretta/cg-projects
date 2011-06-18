/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Mar 2011
 *
 *  MSB Model Loader. Imports a binary mesh file that contain only the VBO.
 */
#pragma once

#include <vector>
#include <string>
#include "MeshLoaders/MeshFileBase.h"

using namespace std;

class MsbMeshFile : public MeshFileBase
{

public:
  /**
   * Simple Constructor
   */
  MsbMeshFile(void);
  ~MsbMeshFile(void);

  /**
   * Read the file of the given name and scales it's vertexes by the scale factor
   */
  virtual void readFile(string fileName, Vector3 pos = Vector3(0,0,0), Vector3 scale = Vector3(1,1,1));

protected:
  /**
   * Calculate the VBO with the read vertexes, normals and indexes.
   */
  virtual void calcVBO();
  
};
