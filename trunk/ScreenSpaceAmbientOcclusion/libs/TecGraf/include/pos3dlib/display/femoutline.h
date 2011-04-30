//* femoutline.h
// rodesp@tecgraf.puc-rio.br
// Tecgraf/PUC-Rio
// Apr 2010

#ifndef P3D_FEM_OUTLINE_RENDERER_H
#define P3D_FEM_OUTLINE_RENDERER_H

#include <pos3dlib/defines.h>
#include "color.h"

class TopModel;
class TpvProperty;
class TpvOutlineEdgesGeometry;
class TpvContourSimpleRenderer;

class POS3DLIB_API P3DFemOutlineRenderer
{
public:
  P3DFemOutlineRenderer ();
  ~P3DFemOutlineRenderer ();
  void SetMesh (TopModel* mesh);
  void SetDisplacement (float factor, TpvProperty* displacement);
  void SetColor (float r, float g, float b, float a);
  void SetWidth (float w);
  void SetPolygonOffset (float factor, float units);
  void Render ();

private:
  P3DColor m_color;
  float m_width;
  float m_polygon_offset_factor;
  float m_polygon_offset_units;
  TpvOutlineEdgesGeometry* m_geometry;
  TpvContourSimpleRenderer* m_renderer;
};

#endif