/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Mar 2011
 *
 *  Generic Model Loader. Imports all the supported model files.
 *  Try to use all the mesh loaders implemented to open the file given.
 */

#pragma once
#include <string>

#include "MathUtils/Vector3.h"
#include "MeshLoaders/MeshFileBase.h"

using namespace std;

class MeshLoader: public MeshFileBase
{
  /**
  * Static array of mesh loaders. 
  * All implemented mesh loader are instanced in this loader.
  */
  static MeshFileBase* s_meshLoaders[];
  
  /**
   * Current mesh loader used to load the model.
   */
  MeshFileBase* m_meshLoader;

public:
  /**
   * Simple Constructor
   */
  MeshLoader();

  /**
   *	Destructor
   */
  virtual ~MeshLoader();

  /**
   * Verify if the filetype given can be loaded
   */
  virtual bool isValidFileType(string filetype);

  /**
   * Read the given file and set the attributes to be applied to the model as translation and scale
   */
  virtual void readFile(string fileName, Vector3 pos = Vector3(0,0,0), Vector3 scale = Vector3(1,1,1));

protected:
  /**
   * Calculate the VBO with the read vertexes, normals and indexes.
   */
  void calcVBO();
};

