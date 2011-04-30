//* femmatoutline.h
// rodesp@tecgraf.puc-rio.br
// Tecgraf/PUC-Rio
// Apr 2010

#ifndef P3D_FEM_MATERIAL_OUTLINE_RENDERER_H
#define P3D_FEM_MATERIAL_OUTLINE_RENDERER_H

#include <pos3dlib/defines.h>
#include "color.h"

class P3DModel;
class TpvProperty;
class TpvMaterialOutlineGeometry;
class TpvOutlineEdgesGeometry;
class TpvContourSimpleRenderer;

class POS3DLIB_API P3DFemMaterialOutlineRenderer
{
public:
  P3DFemMaterialOutlineRenderer ();
  ~P3DFemMaterialOutlineRenderer ();
  void SetModel (P3DModel* model);
  void SetDisplacement (float factor, TpvProperty* displacement);
  void SetColor (float r, float g, float b, float a);
  void SetWidth (float w);
  void SetPolygonOffset (float factor, float units);
  void Render ();

private:
  TpvProperty* ComputeMaterialProperty (P3DModel* m);

private:
  P3DColor m_color;
  float m_width;
  float m_polygon_offset_factor;
  float m_polygon_offset_units;
  TpvProperty* m_material_property;
  TpvMaterialOutlineGeometry* m_geometry;
  TpvContourSimpleRenderer* m_renderer;
};

#endif