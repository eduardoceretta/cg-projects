/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  May 2011
 *
 *  Model Loader. Imports diverse file types.
 *  Depends of the external Library Open Asset Import Library (Assimp), available on http://assimp.sourceforge.net/
 *
 * Types Accepted
 *  Common interchange formats
 *    Collada ( .dae )
 *    Blender 3D ( .blend )
 *    3ds Max 3DS ( .3ds )
 *    3ds Max ASE ( .ase )
 *    Wavefront Object ( .obj )
 *    Stanford Polygon Library ( .ply )
 *    AutoCAD DXF ( .dxf )
 *    LightWave ( .lwo )
 *    Modo ( .lxo )
 *    Stereolithography ( .stl )
 *    AC3D ( .ac )
 *    Milkshape 3D ( .ms3d )
 *   * TrueSpace ( .cob,.scn )
 *
 *  Game file formats
 *    * Valve Model ( .smd,.vta )
 *    Quake I Mesh ( .mdl )
 *    Quake II Mesh ( .md2 )
 *    Quake III Mesh ( .md3 )
 *    Quake III BSP ( .pk3 )
 *    * Return to Castle Wolfenstein ( .mdc )
 *    Doom 3 ( .md5* )
 *    Motion Capture
 *    Biovision BVH ( .bvh )
 *    * CharacterStudio Motion ( .csm )
 *
 *  Other file formats
 *    DirectX X ( .x )
 *    BlitzBasic 3D ( .b3d )
 *    Quick3D ( .q3d,.q3s )
 *    Ogre XML ( .mesh.xml )
 *    Irrlicht Mesh ( .irrmesh )
 *    * Irrlicht Scene ( .irr )
 *    Neutral File Format ( .nff )
 *    Sense8 WorldToolKit ( .nff )
 *    Object File Format ( .off )
 *    PovRAY Raw ( .raw )
 *    Terragen Terrain ( .ter )
 *    3D GameStudio ( .mdl )
 *    3D GameStudio Terrain ( .hmp )
 *    Izware Nendo ( .ndo )
 *
 *  * limited support.
 */
#pragma once
 
#include <vector>
#include <string>
#include "MathUtils/Vector3.h"
#include "MeshLoaders/MeshFileBase.h"

using namespace std;

class AssimpMeshFile : public MeshFileBase
{
  /**
   * Assimp Scene pointer
   */
  const struct aiScene *m_aiscene;

public:
  /**
   * Simple Constructor
   */
  AssimpMeshFile(void);
  /**
   * Destroy the MeshImporter
   */
  ~AssimpMeshFile(void);

  /**
   * Read the file of the given name and scales it's vertexes by the scale factor
   */
  virtual void readFile(string fileName, Vector3 pos = Vector3(0,0,0), Vector3 scale = Vector3(1,1,1));

  /**
   * Verify if the filetype (".type") can be read
   */
  bool isValidFileType(string filetype);
  
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

  /**
   * Recursive function that read the aiNode struct and fill the VBO arrays
   */
  void createVbo(struct aiNode* nd, GLfloat * vList, GLfloat * nList, unsigned int * iList, int &vertexOffset, int &indexOffset, struct aiMatrix4x4* trafo);
  
  /**
   * Recursive function that read the aiNode struct and count the number of vertexes and triangles
   */
  void getModelSizes(struct aiNode* nd, int &nVertices, int &nTriangles);
};


