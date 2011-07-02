/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Mar 2010
 *
 *  Scene file loader and OpenGL scene container
 *  Load Meshes, Materials, Lights and Camera
 */
#pragma once

#include <vector>
#include <string>

#include "MathUtils/Vector3.h"
#include "MathUtils/Color.h"

#include "ScScene/ScCamera.h"
#include "ScScene/ScLight.h"
#include "ScScene/ScMesh.h"
#include "ScScene/ScMaterial.h"

using namespace std;

class ScScene
{
protected:
  /**
   * Indicate if the scene needs to be updated
   */
  bool m_calculated;

  /**
   * OpenGL's Clear Color
   */
  Color m_clear;

  /**
   * OpenGL's Ambient Color
   */
  Color m_ambient;

  /**
   * Materials Array
   */
  vector<ScMaterial> m_materials;

  /**
   * Lights Array
   */
  vector<ScLight> m_lights;

  /**
   * Meshes Array
   */
  vector<ScMesh> m_meshes;

  /**
   * Scene's Initial Camera
   */
  ScCamera m_camera;
  
  /**
   * Indicates if the scene lights are on
   */
  bool m_lightEnabled;

public:

  /**
   * Simple Constructor
   */
  ScScene(){};

  /**
   * Read the given file name and import the objects
   */
  ScScene(string rt4FileName);

  /**
   * Delete created textures
   */
  ~ScScene();

  /**
   * Update Scene Objects if needed
   */
  void configure();

  /**
   * Render scene
   */
  void render();

  /**
   * Get the number of imported lights
   */
  int getNumLights();

  /**
   * Get the number of imported meshes
   */
  int getNumMeshes();

  /**
   * Get the ith mesh imported
   */
  ScMesh* getMeshAt(int i);

  /**
   * Get the clear color of the scene
   */
  Color getClearColor() const;

  /**
   * Get the global ambient color of the scene
   */
  Color getAmbientColor() const;

  /**
   * Get the camera of the scene
   */
  ScCamera* getCamera();

  /**
   * Verify if scene's lights are on
   */
  bool isSceneLightEnabled() const;

  /**
   * Sets the scene's lights on
   */
  void setSceneLightEnabled(bool val);

  /**
   * Configure OpenGL to enable lights when active and
   *  to disable lights when inactive.
   */
  void setLightActive(bool op);

  /**
   * Configure OpenGL to enable materials of given index when active and
   *  to disable lights when inactive.
   */
  void setMaterialActive(bool op, int index);

  /**
   * Render ith mesh
   */
  void renderMesh(int index);

  /**
   * Get Bounding Box info
   */
  Vector3 getSceneBoundingBoxSize() const;
  Vector3 getSceneBoundingBoxCenter() const;
  Vector3 getSceneBoundingBoxMin() const;
  Vector3 getSceneBoundingBoxMax() const;

protected:
  /**
   * Read the attributes from a space separated string.
   *  String format: 
   *   m_clear.r clear.g clear.b ambient.r ambient.g ambient.b
   */
  void readFromStr(char buffer[]);

  /**
   * Read the scene file and import the scene objects
   */
  void readFromFile(string rt4FileName);
};

