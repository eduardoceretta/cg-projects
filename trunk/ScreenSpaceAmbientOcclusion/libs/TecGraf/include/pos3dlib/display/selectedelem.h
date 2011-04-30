//* selectedelem.h
// rodesp@tecgraf.puc-rio.br
// Tecgraf/PUC-Rio
// Apr 2010

#ifndef P3D_SELECTED_ELEM_RENDERER_H
#define P3D_SELECTED_ELEM_RENDERER_H

#include <pos3dlib/defines.h>

class TopModel;
class P3DElemSelection;
class TpvProperty;
class TpvBdryFacesGeometry;
class TpvCohFacetsGeometry;
class TpvContourSimpleRenderer;
class P3DElemVis;
/**
  Renders faces of elements selected from a model.
*/
class POS3DLIB_API P3DSelectedElemRenderer
{
public:
  P3DSelectedElemRenderer ();
  ~P3DSelectedElemRenderer ();
  void SetMesh (TopModel* mesh);
  void SetSelection (P3DElemSelection* selection);
  void SetDisplacement (float factor, TpvProperty* displacement);
  void SetElemVis (P3DElemVis* elemvis, bool complementary = false);
  void SetPolygonOffset (float factor, float units);
  void SetWireframeColor (float r, float g, float b);
  void SetWireframeWidth (float s);
  void SetSurfaceColor (float r, float g, float b);
  void Render ();

private:
  void UpdateElemVis ();

private:
  int m_nelem;
  TopModel* m_mesh;
  TpvBdryFacesGeometry* m_bdry_faces_geom;
  TpvCohFacetsGeometry* m_coh_facets_geom;
  TpvContourSimpleRenderer* m_fcontour_rnd;  
  TpvContourSimpleRenderer* m_coh_fcontour_rnd;  
  float m_polygon_offset_factor;
  float m_polygon_offset_units;
  float m_surface_color[3];
  bool* m_elemvis;
  bool* m_final_elemvis;
  bool m_elemvis_outdated;
};

#endif