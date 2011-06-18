/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Mar 2010
 *
 *  OpenGL Material Attributes Container
 */
#pragma once
#include <string>

#include <GL/glut.h>

#include "MathUtils/Vector3.h"
#include "MathUtils/Color.h"

using namespace std;

class ScMaterial
{
  /**
   * Static number of material counter.
   *  Only counts the number of created materials, do not consider deleted materials
   */
  static int s_materialNum;

protected:
  /**
   * Indicate if the material needs to be updated
   */
  bool m_calculed;

   /**
   * Material's Color Attributes
   *  Ambient is 20% of the diffuse color
   */
  GLfloat m_gAmbient[4];
  GLfloat m_gSpecular[4];
  GLfloat m_gDiffuse[4];

   /**
   * Material's Shininess
   */
  GLfloat m_gShi;

  /**
   * Material attributes
   */
  float m_reflective;
  float m_refractive;
  float m_opacity;
  Color m_specular;
  Color m_diffuse;
  float m_specularExp;

  /**
   * This instance material number
   */
  int m_myMaterialNum;
public:
  /**
   * Simple Constructor
   */
  ScMaterial();

  /**
   * Return the number of the material.
   *  Start at 0.
   */
  int getMyMaterialNumber() const;

  /**
   * Read the attributes from a space separated string.
   *  String format: 
   *    diffuse.r diffuse.g diffuse.b specular.r specular.g specular.b specularExp reflective refractive opacity
   */
  void readFromStr(char buffer[]);

  /**
   * Update Light Attributes if needed
   */
  void configure();

  /**
   * Enable the OpenGL material state
   */
  void render();

};

