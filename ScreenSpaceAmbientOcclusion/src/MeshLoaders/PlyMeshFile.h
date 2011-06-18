/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Mar 2011
 *
 *  PLY Model Loader. 
 *  Imports a text mesh file defined by the Stanford University.
 *  Based on fabraham@tecgraf.puc-rio.br em Oct 2004 implementation of SGPLYLoader
 *  Depends of the ply library.
 */

#pragma once

#include <vector>
#include <string>

#include "MeshLoaders/MeshFileBase.h"

using namespace std;

class PlyMeshFile : public MeshFileBase
{
public:
  /**
   * Simple Constructor
   */
  PlyMeshFile(void);
  ~PlyMeshFile(void);
  
  /**
   * Read the file of the given name and scales it's vertexes by the scale factor
   */
  virtual void readFile(string fileName, Vector3 pos = Vector3(0,0,0), Vector3 scale = Vector3(1,1,1));
  
protected:
  /**
   * Calculate the VBO with the read vertexes, normals and indexes.
   *  Writes the binary file automatically if m_writeBinaryFile is true
   */
   virtual void calcVBO();
  
   /**
   * Import information from file and organize it in the array form needed by
   *  the VBO
   */
   void calcTriangles();
};


