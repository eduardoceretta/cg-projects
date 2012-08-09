/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Aug 2012
 *
 *  Animation Model Loader. Imports diverse file types.
 *  Receives another KeyFrame and then constructs the VBO with the info
 *  needed by the animation interpolation.
 *
 */
#pragma once
 
#include <vector>
#include <string>
#include "MathUtils/Vector3.h"
#include "MeshLoaders/MeshFileBase.h"


using namespace std;

class MeshLoader;

class KeyFrameMesh : public MeshFileBase
{
  MeshLoader* m_meshLoader; // Load the mesh files
  MeshFileBase *m_prevKF;   // Neighbor KeyFrames
  MeshFileBase *m_nextKF;   // Neighbor KeyFrames
public:
  /**
   * Simple Constructor
   */
  KeyFrameMesh();

  /**
   * Destroy the MeshLoader
   */
  virtual ~KeyFrameMesh();

  /**
   * Read the file of the given name and scales it's vertexes by the scale factor.
   *  Use the readFile from MeshLoader.
   */
  virtual void readFile(string fileName, Vector3 pos = Vector3(0,0,0), Vector3 scale = Vector3(1,1,1));

  /**
   * Verify if the filetype (".type") can be read
   *   Use the method from MeshLoader.
   */
  bool isValidFileType(string filetype);

  /**
   *	Set the neighbor KeyFrames
   */
  void setNextKeyFrame(MeshFileBase *n);
  void setPreviousKeyFrame(MeshFileBase *p);
  
protected:
  /**
   * Calculate the VBO with the read vertexes, normals and indexes from this mesh and next key frames.
   *  Write the binary file automatically if m_writeBinaryFile is true
   */
  virtual void calcVBO();
};


