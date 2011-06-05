/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/

/**********************************************************\
Types Accepted
  Common interchange formats
    Collada ( .dae )
    Blender 3D ( .blend )
    3ds Max 3DS ( .3ds )
    3ds Max ASE ( .ase )
    Wavefront Object ( .obj )
    Stanford Polygon Library ( .ply )
    AutoCAD DXF ( .dxf )
    LightWave ( .lwo )
    Modo ( .lxo )
    Stereolithography ( .stl )
    AC3D ( .ac )
    Milkshape 3D ( .ms3d )
    * TrueSpace ( .cob,.scn )

  Game file formats
    * Valve Model ( .smd,.vta )
    Quake I Mesh ( .mdl )
    Quake II Mesh ( .md2 )
    Quake III Mesh ( .md3 )
    Quake III BSP ( .pk3 )
    * Return to Castle Wolfenstein ( .mdc )
    Doom 3 ( .md5* )
    Motion Capture
    Biovision BVH ( .bvh )
    * CharacterStudio Motion ( .csm )

  Other file formats
    DirectX X ( .x )
    BlitzBasic 3D ( .b3d )
    Quick3D ( .q3d,.q3s )
    Ogre XML ( .mesh.xml )
    Irrlicht Mesh ( .irrmesh )
    * Irrlicht Scene ( .irr )
    Neutral File Format ( .nff )
    Sense8 WorldToolKit ( .nff )
    Object File Format ( .off )
    PovRAY Raw ( .raw )
    Terragen Terrain ( .ter )
    3D GameStudio ( .mdl )
    3D GameStudio Terrain ( .hmp )
    Izware Nendo ( .ndo )

  * limited support.
\**********************************************************/

#pragma once

#include <vector>
#include "GraphBasis/Vector3.h"
#include "MeshLoaders/MeshFileBase.h"

#include "Objects/Triangle.h"

using namespace std;

class AssimpMeshFile : public MeshFileBase
{
  string m_fileName;
  const struct aiScene *m_aiscene;
  Vector3 m_bb_min;
  Vector3 m_bb_max;
public:
  AssimpMeshFile(void);
  ~AssimpMeshFile(void);

  virtual void readFile(string fileName, Vector3 pos = Vector3(0,0,0), Vector3 scale = Vector3(1,1,1));
  bool isValidFileType(string filetype);
  
  void writeBinaryFile(string fileName);
protected:
   virtual void calcVBO();
   void calcTriangles();
   void createVbo(struct aiNode* nd, GLfloat * vList, GLfloat * nList, unsigned int * iList, int &vertexOffset, int &indexOffset, struct aiMatrix4x4* trafo);
   void getModelSizes(struct aiNode* nd, int &nVertices, int &nTriangles);
};


