/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Mar 2010
 *
 *  OpenGL Mesh Attributes Container
 *  A VBO/P3BMesh with a material and a transformation
 */

#pragma once
#include <string>

#include "MathUtils/Vector3.h"
#include "MeshLoaders/P3bMeshFile.h"

using namespace std;

class GLVertexBufferObject;

class ScMesh
{
  /**
   * Static number of meshes counter.
   *  Only counts the number of created meshes, do not consider deleted meshes
   */
  static int s_meshNum;

protected:
  /**
   * VBO used for rendering
   */
  GLVertexBufferObject* m_vbo;

  /**
   * P3BModel used for render
   */
  P3bMeshFile * m_p3bMesh;

  /**
   * Indicate if the light needs to be updated
   */
  bool m_calculated;

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
   * This instance mesh number
   */
  int m_myMeshNum;
public:
   /**
   * Simple Constructor
   */
  ScMesh();
  
  /**
   * Delete allocated memory
   */
  ~ScMesh();

  /**
   * Return the number of the mesh.
   *  Start at 0.
   */
  int getMyMeshNumber() const;

  /**
   * Read the attributes from a space separated string.
   *  String format: 
   *   materialIndex pos.x pos.y pos.z scale.x scale.y scale.z
   */
  void readFromStr(char buffer[]);

  /**
   * Getters of Mesh Attributes
   */
  int getMaterialIndex();
  GLVertexBufferObject* getVbo() const;
  P3bMeshFile * getP3bMesh() const;

  /**
   * Update Light Attributes if needed
   */
  void configure();
  
  /**
   * Render the transformed VBO and P3BMesh
   */
  void render();
};

