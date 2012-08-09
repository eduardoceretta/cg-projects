/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Aug 2012
 *
 *  Mesh Animation Container
 *  Several key frames with a material and a transformation
 */

#pragma once
#include <string>
#include <vector>

#include "MathUtils/Vector3.h"

using namespace std;

class GLAnimationObject;
class KeyFrameMesh;

class ScAnimMesh
{
  /**
   * Static number of animmeshes counter.
   *  Only counts the number of created meshes, do not consider deleted meshes
   */
  static int s_animMeshNum;

protected:
  /**
   * Key Frames and animation used for rendering
   */
  vector<KeyFrameMesh* > m_keyFrameMeshes;
  GLAnimationObject* m_animObj;

  /**
   * Material associated with the mesh
   */
  int m_materialIndex;

  /**
   * World Position to translate
   */
  Vector3 m_pos;

  /**
   * Model Scale
   */
  Vector3 m_scale;

  /**
   *	Animation Parameters
   *    Speed in fps
   *    AnimType: Regular, Reverse, Loop
   */
  float m_animSpeed; 
  string m_animType;

  /**
   * Mesh File Names
   */
  vector<string> m_keyFrameFileNames;
  
  /**
   * This instance mesh number
   */
  int m_myAnimMeshNum;

  /**
   *	Is the animation setup done
   */
  bool m_animOutdated;

public:
   /**
   * Simple Constructor
   */
  ScAnimMesh();
  
  /**
   * Delete allocated memory
   */
  ~ScAnimMesh();

  /**
   * Return the number of the mesh.
   *  Start at 0.
   */
  int getMyAnimMeshNumber() const;

  /**
   * Read the attributes from a space separated string.
   *  String format: 
   *   materialIndex pos.x pos.y pos.z scale.x scale.y scale.z (fileNames...)
   */
  void readFromStr(char buffer[]);

  /**
   * Getters of AnimMesh Attributes
   */
  GLAnimationObject* getAnimationObject() const;
  int getMaterialIndex();
  string getFileName(int i) const;
  string getFullFilePath() const;

  /**
   * Get Bounding Box info
   */
  Vector3 getBoundingBoxSize() const;
  Vector3 getBoundingBoxCenter() const;
  Vector3 getBoundingBoxMin() const;
  Vector3 getBoundingBoxMax() const;

  /**
   * Update Animation Attributes
   */
  void configure();
  
  /**
   * Render the transformed Animation
   */
  void render();
};

