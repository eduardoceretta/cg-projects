/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Mar 2011
 *
 *  P3B Model Loader. 
 *  Imports a binary mesh file that load a FiniteElement Model used 
 *  in TecGraf's Pos3D.
 *  Permits the render of this model.
 *  Depends on diverse P3D libs.
 */
#pragma once
#include <string>
#include "MathUtils/Vector3.h"

using namespace std;

class P3DFemDisplay;
class P3DModel;
class P3DNfrModelReader;
class P3DElemSelection;
class P3DElemVis;

class P3bMeshFile
{
  /**
   * Accepted model File Type
   */
  static string s_fileType;

  /**
   * File Name to import
   */
  string m_fileName;

  /**
   * Position to translate
   */
  Vector3 m_pos;

  /**
   * Value to scale the model render
   */
  Vector3 m_scale;

  /**
   * P3D model pointer
   */
  P3DModel *m_p3dModel;

  /**
   * P3D Finite Element Neutral File Reader
   */
  P3DNfrModelReader *m_p3dModelReader;

  /**
   * P3D Model Renderer
   */
  P3DFemDisplay* m_p3dRenderer;

  /**
   * P3D visibility of elements handler
   */
  P3DElemVis *m_elemvis;

  /**
   * P3D selection of elements
   */
  P3DElemSelection *m_elemsel;

public:
  /**
   * Simple Constructor
   */
  P3bMeshFile(void);

  /**
   * Destroy the allocated objects
   */
  ~P3bMeshFile(void);

  /**
   * Get the number of elements of the P3DModel
   */
  int getNumElements();

  /**
   * Set the array of booleans indicating if each element is visible.
   *  True means visible, false invisible.
   */
  void setVisibleElements(bool *v);

  /**
   * Read the given file and set the attributes to be applied to the model as translation and scale
   */
  void readFile(string fileName, Vector3 pos = Vector3(0,0,0), Vector3 scale = Vector3(1,1,1));

  /**
   * Render the model with the respective visibility set.
   */
  void render();

  /**
   * Compares the given filetype(".typ") with the loader accepted type.
   *  Return true if is a valid file type
   */
  static bool isValidFileType( string filetype );
protected:
};


