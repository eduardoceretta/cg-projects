
#ifndef TPV_POLY_CONTOUR_SHADER_RENDERER_3_H
#define TPV_POLY_CONTOUR_SHADER_RENDERER_3_H

class CGShader;

#include <ds/list.h>

#include "../geometry/facetoshadertri.h"
#include "../geometry/linegeometry.h"
#include "../property/property.h"
#include "../colorscale/colorscale.h"
#include "renderer3.h"
#include <topsview/defines.h>

/**
    Polygonal contour mapper. Maps triangle, line and point
  scalars to colors.
*/
MODEL_CLASS_TEMPLATE class TPV_API TpvPolyContourShaderRenderer3 : public TpvRenderer3
{
private:
  typedef DsList<TpvFaceToShaderTriGeometry<MODELCLASS>*> TriList;
  typedef DsList<TpvLineGeometry3<MODELCLASS>*> LineList;
  TriList m_tri_geoms;
  LineList m_line_geoms;
  TopModel* m_model;
  TpvProperty* m_property;
  TpvColorScale* m_colorscale;
  int m_base_id;
  int m_nsteps;
  int m_curr_step;

  bool m_update_geometry;
  bool m_update_property;
  bool m_update_colorscale;

  bool m_initialized;

  float m_clip_plane[4];
  bool  m_clip_plane_enabled;
  float m_poly_offset_factor;
  float m_poly_offset_units;
  bool  m_poly_offset_enabled;

  bool m_color_mask_enabled;

  bool m_render_triangles;
  bool m_render_lines;
  bool m_render_points;

  unsigned int m_tex_obj;
  unsigned int m_cs_tex_obj;
  int          m_tex_w;
  float*       m_tex_data;   
  float        m_tex_matrix[16];

  char        m_shader_path[256];
  CGShader*   m_shader_q4; 
  CGShader*   m_shader_q8; 

  void LoadTexture();
  void UnloadTexture();
  void DrawArray(int n, int dim, 
    float* pos, float* normal, float* scalar);
  void DrawArrayQ(int n,
    float* pos, float* normal, float* param, float* scalar, float* scalar2 = NULL);
  void RenderTrianglesQ4();
  void RenderTrianglesQ8();
  void RenderTriangles();
  void RenderLines();
  void RenderPoints();

public: 
  TpvPolyContourShaderRenderer3();
  virtual ~TpvPolyContourShaderRenderer3();

  void SetShaderPath(const char* path);
  bool Init();

  void AddTriGeometry(TpvFaceToShaderTriGeometry<MODELCLASS>* g);
  void ClearTriGeometries();
  void AddLineGeometry(TpvLineGeometry3<MODELCLASS>* g);
  void ClearLineGeometries();
  void ClearGeometries()
  {
    ClearTriGeometries();
    ClearLineGeometries();
  }

  /** Sets the property data used to render geometry. */
  virtual void SetProperty(TpvProperty* property);
  virtual void SetColorScale(TpvColorScale* colorscale);

//TMP para usar a topsview nova no pos3d.
  void SetColorScaleTexObj(int tex_obj);

  virtual void SetBaseId(int id);
  virtual int GetBaseId();

  virtual void SetNumOfSteps(int n);
  virtual void SetCurrentStep(int step_id);

  virtual void SetClippingPlane(float a, float b, float c, float d);
  virtual void SetClippingPlaneEnabled(bool flag);

  virtual void SetPolygonOffset(float factor, float units);
  virtual void SetPolygonOffsetEnabled(bool flag);

  /** 
      If color mask is disabled, geometries will be rendered to the
    depth-buffer only.    
  */
  void SetColorMaskEnabled(bool flag);

  void SetRenderTriangles(bool flag);
  void SetRenderLines(bool flag);
  void SetRenderPoints(bool flag);

  virtual bool Update();
  virtual void Render();
private:
  static int InitExtensions ();
};

#include "polycontourshader3.inl"

#endif
