//* femboundary.h
// rodesp@tecgraf.puc-rio.br
// Tecgraf/PUC-Rio
// Apr 2010

#ifndef P3D_FEM_BOUNDARY_RENDERER_H
#define P3D_FEM_BOUNDARY_RENDERER_H

class UGLTriangleSet;
class TpvColorScale;
class TpvBdryEdgesGeometry;
class TpvBdryFacesBilinearGeometry;
class TpvBdryFacesGeometry;
class TpvBdryNodesGeometry;
class TpvCohEdgesGeometry;
class TpvCohFacetsBilinearGeometry;
class TpvCohFacetsGeometry;
class TpvCohNodesGeometry;
class TpvContourBilinearRenderer;
class TpvContourSimpleRenderer;
class TpvCutGeometry;
class P3DFemGeomSortManager;
class TpvLineSet;
class TpvNumbersRenderer;
class TpvProperty;
class TpvTriangleSet;
class SciColorScale;
class SciColorScaleTex;

#include <pos3dlib/defines.h>
#include "../model/model.h"
#include "../elemvis.h"
#include "color.h"
#include "femgeomsortmanager.h"
#include <alg/plane.h>
#include <ugl/texture.h>

#define P3D_FEM_BOUNDARY_USE_COLORSCALE_TEX

#ifdef P3D_FEM_BOUNDARY_USE_COLORSCALE_TEX
#include <sciutils/colorscaletex.h>
#endif

class POS3DLIB_API P3DFemBoundaryRenderer
{
public:
  enum FaceMode {
    FACE_COLOR = 0,
    FACE_PER_ELEMENT_COLORS,
    FACE_CONTOUR,
    FACE_MATERIALS
  };

  enum MeshMode {
    MESH_COLOR = 0,
    MESH_CONTOUR
  };

  enum SelectionMode {
    SELECT_NONE = 0,
    SELECT_ELEMENT = 1,
    SELECT_NODE = 2,
    SELECT_FACE = 4
  };

  enum CullFaceMode {
    CULL_FACE_NONE = 0,
    CULL_FACE_FRONT,
    CULL_FACE_BACK
  };

  P3DFemBoundaryRenderer ();

  virtual ~P3DFemBoundaryRenderer ();

  /** 
    Sets the path where to look for shaders. 
  */
  void SetShaderPath (const char* path);

  void SetMesh (TopModel* mesh);

  /**
    Sets the array of element visibility.
    Default: NULL (all elements are visible)
    If complementary is true the elemvis represents
    the complementary visibility.
  */
  void SetElemVis (P3DElemVis* elemvis, bool complementary = false);

  /** 
    Sets the current time-step. 
  */
  void SetCurrentStep (int id);

  void SetRezone (int nsteps, int* rezone);

  /** 
    Sets the current model result. 
  */
  void SetFaceScalarField (TpvProperty* field);

  void SetMeshScalarField (TpvProperty* field);

  void SetMaterials (TpvProperty* prop);

  /** 
    Displacement used to render deformed meshes. 
  */
  void SetDisplacement (float factor, TpvProperty* displacement);

  /*****************************
    Faces
  ******************************/
  void SetFacesEnabled (bool flag);

  void SetFaceMode (FaceMode mode);

  bool IsFaceModeSupported (FaceMode mode);

  void SetBilinearEnabled (bool flag);

  bool IsBilinearSupported ();

  /**
   * Turns on/off iso-line drawing.
   */
  void SetIsoLinesEnabled (bool flag);

  /** 
    Sets the current color scale.
    The alpha component is ignored (considered as 1).
    A model opacity can be separately set by using
    SetOpacityFactor() method.
  */
  void SetColorScale (SciColorScale* colorscale);

  /**
    Sets a global opacity factor, in the [0,1] interval, for
    every color in the color scale.
    If less than 1.0, model transparency will be enabled.
    Default: 1.0
  */
  void SetOpacityFactor (float factor);

  /**
    Sets the color scale used for rendering of element materials. 
    If color scale mode is DISCRETE, then the distinct discrete colors
    will be used for rendering each material. Otherwise the materials will
    be just mapped onto the colors in the scale.

    Calling this method overrides the current material colors set (see
    SetMaterialColors()).
  */
  void SetMaterialColorScale (SciColorScale* colorscale);

  /**
    Sets discrete material colors. 
    
    Calling this method overrides the current material color scale (see
    SetMaterialColorScale()).
   */
  void SetMaterialColors (int ncolors, const float* colors_rgba);

  /**
    Sets the default color used for solid element rendering.
    The element color is ignored when result contouring is enabled.
  */
  void SetElemColor (float r, float g, float b, float a);

  /**
    Sets the default color used for interface element rendering.
    The element color is ignored when result contouring is enabled.
  */
  void SetCohElemColor (float r, float g, float b, float a);

  /**
    Sets the colors used in per-elem colors rendering mode.
    The current colors will be invalidated if a new model with a 
    different number of element is set. If there are no valid per-element 
    colors, the current global element colors (set by SetElemColor()) will be used
  */
  void SetPerElemColors (TpvProperty* colors);

  /**
    Sets face culling mode.
    Default: NONE (draws every face)
  */
  void SetCullFaceMode (CullFaceMode mode);

  /*****************************
    Mesh
  ******************************/
  void SetMeshEnabled (bool flag);

  void SetMeshMode (MeshMode mode);

  bool IsMeshModeSupported (MeshMode mode);

  /** Solid element mesh. */
  void SetMeshColor (float r, float g, float b, float a);

  void SetMeshWidth (float w);

  /** Interface element mesh. */
  void SetCohMeshColor (float r, float g, float b, float a);

  void SetCohMeshWidth (float w);

  /******************************
    Nodes
  *******************************/
  void SetNodesEnabled (bool flag);

  void SetNodesColor (float r, float g, float b, float a);

  void SetCohNodesColor (float r, float g, float b, float a);

  void SetNodesSize (float s);

  /*******************************
    Node numbers
  ********************************/
  void SetNodeNumberFont(const char *family_name, int size, bool bold = false, bool italic = false);
  
  void SetNodeNumbersEnabled (bool flag);

  void SetNodeNumbersColor (float r, float g, float b, float a);

  /*******************************
    Element numbers
  ********************************/
  void SetElemNumberFont(const char *family_name, int size, bool bold = false, bool italic = false);

  void SetElemNumbersEnabled (bool flag);

  void SetElemNumbersColor (float r, float g, float b, float a);

  /*******************************
    Cutting plane
  ********************************/
  bool IsCuttingPlaneSupported ();

  void SetCuttingPlaneEnabled (bool flag);

  void SetCuttingPlane (AlgPlane plane);

  void SetCuttingPlaneSliceEnabled (bool flag);

  TpvTriangleSet* GetCuttingPlaneTriangles ();

  TpvLineSet* GetEdges ();

  TpvLineSet* GetCohEdges ();

  /**
      Sets selection mode, defined by enum SelectionMode.
  */
  void SetSelectionMode (unsigned mode);

  /** Render the model. */
  void Render ();

  /** Redraws the model in selection mode. */
  void RenderSelection ();

  void RenderElemHandles (int vpx, int vpy, int vpw, int vph);

private:
  void SetTransparencyEnabled (bool flag);
  void RebuildSortManager ();
  TpvProperty* GetCurrentFaceScalarField ();
  TpvProperty* GetCurrentMeshScalarField ();
  FaceMode GetRenderFaceMode ();
  MeshMode GetRenderMeshMode ();
  void Update ();
  void UpdateScalarField ();
  void UpdateColorScale ();
  void UpdateScalarFieldColorScale ();
  void UpdateMaterialColorScale ();
  void LoadClippingPlane ();
  void UnloadClippingPlane ();
  void LoadCullFace ();
  void UnloadCullFace ();
  void LoadContourColorScale(FaceMode mode, bool cohesive);
  void UnloadContourColorScale(FaceMode mode);
  void RenderSolidElemContour(
    FaceMode mode, bool lighting, bool faces, bool mesh,
    bool mesh_depth_test);
  void RenderCohElemContour(
    FaceMode mode, bool lighting, bool faces, bool mesh);
  void RenderSolidElemContourShader(FaceMode mode, bool lighting);
  void RenderCohElemContourShader(FaceMode mode, bool lighting);
  void RenderCutContour(FaceMode mode, bool lighting, bool faces, bool mesh);
  void RenderElemMesh();
  void RenderCohElemMesh();
  void RenderNodes();
  void RenderVirtualNodes();
  void RenderVirtualNodesSelection();
  void RenderElemNumbers();
  void RenderNodeNumbers();  
  TpvColorScale* ComputeTpvColorScale (SciColorScale* scicolorscale);
  AlgMatrix GetTexLimitsTransform (float vmin, float vmax);  

private:
  TopModel* m_mesh;
  char m_shader_path[1024];
  TpvProperty* m_face_scalar_field;
  TpvProperty* m_mesh_scalar_field;
  TpvProperty* m_materials;
  TpvProperty* m_per_elem_colors;
  FaceMode m_face_mode;
  MeshMode m_mesh_mode;
  CullFaceMode m_cull_face_mode;
  int m_selection_mode;
  int m_num_material_colors;
  bool m_isopaque;
  bool m_faces_enabled;
  bool m_mesh_enabled;
  bool m_nodes_enabled;
  bool m_elem_numbers_enabled;
  bool m_node_numbers_enabled;
  bool m_transparency_enabled;
  bool m_cutplane_enabled;
  bool m_cutplane_slice_enabled;
  bool m_bilinear_enabled;
  bool m_update_scalar_field;
  bool m_update_colorscale;
  bool m_isolines;
  bool m_discrete_material_colors;
  AlgPlane m_cutplane;
  P3DColor m_elem_color;
  P3DColor m_coh_elem_color;
  P3DColor m_mesh_color;
  P3DColor m_coh_mesh_color;
  P3DColor m_nodes_color;
  P3DColor m_coh_nodes_color;
  float m_nodes_size;
  P3DColor m_node_numbers_color;
  P3DColor m_coh_node_numbers_color;
  P3DColor m_elem_numbers_color;
  P3DColor m_coh_elem_numbers_color;
  float m_opacity_factor;
  float m_mesh_width;
  float m_coh_mesh_width;
  DsArray<float> m_material_colors;
  float* m_colorscale_tex_data;
  float* m_material_tex_data;
  SciColorScale* m_colorscale;
  SciColorScale* m_material_colorscale;

  P3DFemGeomSortManager m_sort_manager;

  TpvBdryFacesGeometry* m_bdry_faces_geom;
  TpvBdryEdgesGeometry* m_bdry_edges_geom;
  TpvCohFacetsGeometry* m_coh_facets_geom;
  TpvCohEdgesGeometry* m_coh_edges_geom;
  TpvBdryNodesGeometry* m_bdry_nodes_geom;
  TpvCohNodesGeometry* m_coh_nodes_geom;
  TpvCutGeometry* m_cut_geom;
  TpvBdryFacesBilinearGeometry* m_bdry_faces_bilinear_geom;
  TpvCohFacetsBilinearGeometry* m_coh_facets_bilinear_geom;

  TpvContourSimpleRenderer* m_contour_rnd;  
  TpvContourSimpleRenderer* m_coh_contour_rnd;  
  TpvContourSimpleRenderer* m_bdry_edges_rnd;
  TpvContourSimpleRenderer* m_coh_edges_rnd;  
  TpvContourSimpleRenderer* m_bdry_nodes_rnd;  
  TpvContourSimpleRenderer* m_coh_nodes_rnd;  
  TpvContourSimpleRenderer* m_cut_rnd;  
  TpvContourBilinearRenderer* m_contour_bilinear_rnd;
  TpvContourBilinearRenderer* m_coh_contour_bilinear_rnd;

  TpvNumbersRenderer* m_elem_numbers_rnd;
  TpvNumbersRenderer* m_coh_elem_numbers_rnd;
  TpvNumbersRenderer* m_node_numbers_rnd;
  TpvNumbersRenderer* m_coh_node_numbers_rnd;

#ifdef P3D_FEM_BOUNDARY_USE_COLORSCALE_TEX
  SciColorScaleTex m_colorscale_tex;
#else
  UGLTexture m_colorscale_tex;
#endif
  SciColorScaleTex m_material_colorscale_tex;
  UGLTexture m_material_tex;
};

#endif