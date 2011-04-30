//* femgeomsortmanager.h
// rodesp@tecgraf.puc-rio.br
// Tecgraf/PUC-Rio
// Apr 2010

#ifndef P3D_FEM_GEOM_SORT_MANAGER_H
#define P3D_FEM_GEOM_SORT_MANAGER_H

#include <ds/array.h>
#include <pos3dlib/defines.h>

class TpvTriangleSet;
class UGLTriangleSet;

class POS3DLIB_API P3DFemGeomSortManager 
{
public:
  P3DFemGeomSortManager ();
  ~P3DFemGeomSortManager ();
  void Add (TpvTriangleSet* g);
  void Clear ();
  void SetEnabled (bool flag);
  void SetEyePos (float x, float y, float z);
  void Update ();

private:
  DsArray<TpvTriangleSet*> m_geomsets;
  DsArray<UGLTriangleSet*> m_indices;
  float m_ex, m_ey, m_ez;
  bool m_update;
  bool m_enabled;
};


#endif