/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Mar 2010
 *
 *  OpenGL Light Attributes Container
 */
#pragma once

#include <string>
#include "assert.h"

#include "MathUtils/Vector3.h"
#include "MathUtils/Color.h"

#include "GLLights/GLLight.h"

using namespace std;

/**
 * Light Struct defined to be dumped into a file or GL Texture
 *  lightStruct - as accessed by a shader texture
 *  {
 *  0|   ambient  //alpha == enable or disable 
 *  1|   diffuse  //alpha == spotExponent
 *  2|   specular //
 *  3|   pos      //w == type :0 directional, 1 point, 2 = spot
 *  4|   spot     //rgb == spotDir, a == spotAngle(rad)
 *  }
 */
struct lightStruct
{
  GLfloat diffuse[3];
  GLfloat spotExponent;
  GLfloat specular[3];
  GLfloat enabled;
  GLfloat pos[3];
  GLfloat type;
  GLfloat spotDir[3];
  GLfloat spotAngle;
};

class ScLight
{
  /**
   * Static number of lights counter.
   *  Only counts the number of created lights, do not consider deleted lights
   */
  static int s_lightNum;

  /**
   * Type of Lights supported
   */
  enum LightType {Directional = 0, Point = 1, Spot = 2 };

protected:
  /**
   * Light Used for rendering
   */
  GLLight *m_GLLight;

  /**
   * Indicate if the light needs to be updated
   */
  bool m_calculed;

  /**
   * Light's World Position
   */
  Vector3 m_pos;

  /**
   * Light's Color Attributes
   */
  Color m_specular;
  Color m_diffuse;

  /**
   * Light Type
   *  see enum LightType
   */
  LightType m_type;

  /**
   * Attributes of a spot light if light type is Spot
   */
  Vector3 m_spotDir;
  float m_spotExponent;
  float m_spotAngle;

  /**
   * This instance light number
   */
  int m_myLightNum;

  /**
   * Pointer to light structure to be dumped 
   */
  struct lightStruct * m_lightStruct;

public:
  /**
   * Definition of max accepted lights
   */
  enum {Max_Lights = 8};

  /**
   * Simple Constructor
   */
  ScLight();

  /**
   * Delete allocated memory
   */
  ~ScLight();

   /**
   * Return the number of the light.
   *  Start at 0.
   */
  int getMyLightNumber() const;

  /**
   * Read the attributes from a space separated string.
   *  String format: 
   *    pos.x pos.y pos.z diffuse.r diffuse.g diffuse.b specular.r specular.g specular.b (Needed)
   *    type (If not defined type is Point)
   *    spotExponent spotAngle spotDir.x spotDir.y spotDir.z (Only needed if type is Spot)
   */
  void readFromStr(char buffer[]);

  /**
   * Update Light Attributes if needed
   */
  void configure();

  /**
   * Render the GLLight
   */
  void render();

  /**
   * Get the pointer to the Light Structure
   */
  struct lightStruct * getLightStruct();
};

