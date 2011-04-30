/**************************************************************
 **************************************************************/

#ifndef P3D_FEM_DISPLAY_2_H
#define P3D_FEM_DISPLAY_2_H

#include <vgl/vgl.h>
#include <alg/matrix.h>
#include <ugl/texture.h>
#include <sciutils/colorscale.h>
#include <topsview/colorscale/colorscale.h>
#include <topsview/property/property.h>
#include <topsview/geometry/geomsetbase.h>
#include "../model/model.h"
#include "../nodeselection.h"
#include "../elemselection.h"
#include "../elemvis.h"
#include "display.h"

class UGLTriangleSet;
class TpvTriangleSet;
class TpvLineSet;
class P3DPixelRect;
class P3DSelectedElemRenderer;
class P3DSelectedNodesRenderer;
class P3DFemBoundaryRenderer;
class P3DFemMaterialOutlineRenderer;
class P3DFemOutlineRenderer;
class P3DFemVirtualNodesRenderer;
class P3DFemVolumeRenderer;
class P3DFemGeomSortManager;

class P3DFemDisplay2 : public P3DDisplay
{
public:
  typedef P3DDisplay SuperClass; 

  enum SelectionMode {
    SELECT_NONE = 0,
    SELECT_ELEMENT = 1,
    SELECT_NODE = 2,
    SELECT_FACE = 4,
    SELECT_VIRTUAL_NODE = 8
  };

  P3DFemDisplay2 ();
  virtual ~P3DFemDisplay2 ();

  /** Sets the path where to look for shaders. */
  void SetShaderPath(const char* path);

  /** Current model. */
  void SetModel(P3DModel* model);
  P3DModel* GetModel()
  {
    return m_model;
  }

  /** Current model result. */
  void SetScalarResult(P3DResult* result);
  void SetSmoothScalarResult(P3DResult* result);

  /** Displacement used to render deformed meshes. */
  void SetDisp(P3DResult* disp);
  void SetDispFactor(float factor);
  void SetUndeformedEnabled(bool flag);

  void SetColorScale(SciColorScale* colorscale);

  /**
    Sets the colors used for rendering of materials.
  */
  void SetMaterialColorScale(SciColorScale* colorscale);

  /**
    Sets the array of element visibility.
    Default: NULL (all elements are visible)
  */
  void SetElemVis(P3DElemVis* elemvis);

  /**
    Sets the array of selected nodes.
    Default: NULL (disabled)
  */
  void SetNodeSelection(P3DNodeSelection* nodesel);

  void SetNodeSelectionColor(float r, float g, float b);
  void SetNodeSelectionSize(float size);
  /**
    Sets the array of selected elements.
    Default: NULL (disabled)
  */
  void SetElemSelection(P3DElemSelection* elemsel);

  void SetElemSelectionSurfaceColor(float r, float g, float b);
  void SetElemSelectionWireframeColor(float r, float g, float b);
  void SetElemSelectionWireframeWidth(float s);

  /** Extrusion for 2D meshes. */
  void SetExtrudedEnabled(bool flag);
  bool GetExtrudedEnabled();

  /** Cutting plane. */
  void SetCuttingPlaneEnabled(bool flag);
  void SetCuttingPlane(float a, float b, float c, float d);

  /** Probe box. */
  bool IsProbeBoxSupported();
  void SetProbeBoxEnabled(bool flag);
  void SetProbeBox(float xmin, float xmax, float ymin, float ymax,float zmin, float zmax);


  /*******************************************************
    Boundary
  ********************************************************/
  void SetBoundaryEnabled(bool flag);

  /************** Faces ***************************/
  enum FaceMode {
    FACE_COLOR = 0,
    FACE_CONTOUR,
    FACE_MATERIALS
  };

  enum CullFaceMode {
    CULL_FACE_NONE = 0,
    CULL_FACE_FRONT,
    CULL_FACE_BACK
  };

  void SetBoundaryFacesEnabled(bool flag);

  void SetBoundaryFaceMode(FaceMode mode);

  void SetBoundaryFaceSmoothEnabled(bool f);

  bool SetBoundaryBilinearEnabled(bool f);

  void SetBoundaryOpacityFactor(float f);

  /**
    Sets the default color used for solid element rendering.
    The element color is ignored when result contouring is enabled.
  */
  void SetBoundaryElemColor(float r, float g, float b, float a);

  /**
    Sets the default color used for interface element rendering.
    The element color is ignored when result contouring is enabled.
  */
  void SetBoundaryCohElemColor(float r, float g, float b, float a);

  void SetIsoLinesEnabled(bool flag);

  void SetBoundaryCullFaceMode (CullFaceMode mode);

  /************** Mesh ***************************/
  void SetBoundaryMeshEnabled(bool flag);

  /**
    Sets whether mesh contouring for solid elements is enabled.
    Returns whether the display mode is supported.
  */
  bool SetBoundaryMeshContourEnabled(bool flag);
  void SetBoundaryMeshColor(float r, float g, float b, float a);
  void SetBoundaryMeshWidth(float w);
  void SetBoundaryCohMeshColor(float r, float g, float b, float a);
  void SetBoundaryCohMeshWidth(float w);

  /******************** Nodes ********************************/
  void SetBoundaryNodesEnabled(bool flag);
  void SetBoundaryNodesColor(float r, float g, float b, float a);
  void SetBoundaryNodesSize (float s);
  void SetBoundaryCohNodesColor(float r, float g, float b, float a);  

  /******************* Elem numbers *****************************/
  void SetBoundaryElemNumbersEnabled(bool flag);
  void SetBoundaryElemNumberFont(const char *family_name, int size, bool bold = false, bool italic = false);
  void SetBoundaryElemNumberColor(float r, float g, float b, float a);

  /******************* Node numbers *****************************/
  void SetBoundaryNodeNumbersEnabled(bool flag);
  void SetBoundaryNodeNumberFont(const char *family_name, int size, bool bold = false, bool italic = false);
  void SetBoundaryNodeNumberColor(float r, float g, float b, float a);

  void SetBoundaryAffectedByCuttingPlane(bool flag);

  TpvTriangleSet* GetBoundaryCuttingPlaneTriangles();
  TpvLineSet* GetBoundaryEdges();
  TpvLineSet* GetBoundaryCohEdges();

  /*******************************************************
    Hidden boundary
  ********************************************************/
  void SetHiddenEnabled(bool flag);

  /************** Faces ***************************/
  void SetHiddenFacesEnabled(bool flag);
  void SetHiddenFaceMode(FaceMode mode);
  void SetHiddenFaceSmoothEnabled(bool f);
  bool SetHiddenBilinearEnabled(bool f);
  void SetHiddenOpacityFactor(float f);
  void SetHiddenElemColor(float r, float g, float b, float a);
  void SetHiddenCohElemColor(float r, float g, float b, float a);

  /************** Mesh ***************************/
  bool SetHiddenMeshContourEnabled(bool flag);  
  void SetHiddenMeshEnabled(bool flag);
  void SetHiddenMeshColor(float r, float g, float b, float a);
  void SetHiddenMeshWidth(float w);
  void SetHiddenCohMeshColor(float r, float g, float b, float a);
  void SetHiddenCohMeshWidth(float w);

  void SetHiddenAffectedByCuttingPlane(bool flag);
  void SetHiddenBoundaryCuttingPlane(bool flag);

  /********************************************************
    Virtual nodes. 
  *********************************************************/
  void SetVirtualNodesEnabled(bool flag);
  void SetVirtualNodesColor(float r, float g, float b, float a);
  void SetVirtualNodesSize(float size);

  /********************************************************
    Outline
  *********************************************************/
  void SetOutlineEnabled(bool flag);
  void SetOutlineColor(float r, float g, float b, float a);
  void SetOutlineWidth(int w);

  /********************************************************
    Material outline
  *********************************************************/
  void SetMaterialOutlineEnabled(bool flag);

  /********************************************************
    Volumetric
  *********************************************************/
  enum VolumetricMode {
    VOLUMETRIC_OFF = 0,
    VOLUMETRIC_RESULT
  };

  void SetVolumetricEnabled(bool flag);

  void SetVolumetricAffectedByCuttingPlane(bool flag);
  /**
    Sets the scale factor which multiplies color opacity when
    transparency is enabled.
    Default: 1.0f
  */
  void SetOpacityScale(float v);

  /**
    If true, the opacity factor used in volumetric mode is scaled so that 
    variation of color scale alpha values is linear when the distance
    travered by a ray inside the model and the opacity are equal to 1.
    If true, the opacity scale (see SetOpacityScale()) should be inversely
    proportional to a reference distance (e.g. 
    scale = 1 / min_bounding_box_dimension).
    Default: true
  */
  void SetLinearizeOpacityEnabled(bool flag);

  void SetVolumetricMode(int mode);

  bool IsVolumetricSupported();

  /********************************************************
    Isosurfaces
  *********************************************************/
  /**
    Currently, isosurface mode is implemented on top of volumetric renderer.
    If any mode, rather then ISOSURFACE_OFF is set, the volumetric mode is
    ignored and results are rendered as isosurfaces.
  */
  void SetIsosurfacesEnabled(bool flag);

  bool AreIsosurfacesSupported();

  void SetIsosurfaceValuesEnabled (bool flag);

  void SetIsosurfaceValues(int nvalues, const float* values);

  float GetMinNormalizedIsosurfaceValueInterval ();

  /** Current view port. Must be updated whenever the view port changes. */
  void SetViewport(int x0, int y0, int w, int h);
  void GetViewport(int* x0, int* y0, int* w, int* h);

  /**
      Sets selection mode, defined by enum SelectionMode.
  */
  void SetSelectionMode(unsigned mode);
  unsigned GetSelectionMode() const
  {
    return m_selection_mode;
  }

  /** Sets the current time-step. */
  void SetCurrentStep(int step_id);
  int GetCurrentStep();

  /**
    Draws handles of elements associated to visible boundary faces
    into a pixel rectangle corresponding to a sub-area of the given
    subcanvas. Pixel rectangle coordinates are relative to the
    subcanvas' viewport. Pixels with no element drawn will
    contain an invalid handle value (TOP_INVALID_HANDLE).

    Returns whether rendering could be done.
  */
  bool DrawElemHandlesToPixelRect (VglSubCanvas* subcanvas, 
    P3DPixelRect* pixelrect);

  /** Resets display to default values. */
  void Reset() {}

  /**
      Requests updating of the display. It will actually be updated
    in the next redraw.    
  */
  void Update();

  /** Render the model. */
  void Render();

  /** Redraws the model in selection mode. */
  void RenderSelection();

private:
  enum MeshMode {
    MESH_COLOR = 0,
    MESH_CONTOUR
  };

  static int GetTpvPropType(int p3d_prop_type)
  {
    switch (p3d_prop_type) {
      case P3DResultField::NODAL:
        return TpvProperty::PROP_NODE;
      case P3DResultField::ELEMENT_NODAL:
        return TpvProperty::PROP_ELEMENT_NODE;
      case P3DResultField::ELEMENT_NODAL_TO_NODAL:
        return TpvProperty::PROP_NODE;
      case P3DResultField::SMOOTH_ELEMENT_NODAL:
        return TpvProperty::PROP_ELEMENT_NODE;
      case P3DResultField::ELEMENT_GAUSS_TO_ELEMENT_NODAL:
        return TpvProperty::PROP_ELEMENT_NODE;
      case P3DResultField::ELEMENT_GAUSS_TO_NODAL:
        return TpvProperty::PROP_NODE;
      case P3DResultField::SMOOTH_ELEMENT_GAUSS_TO_ELEMENT_NODAL:
        return TpvProperty::PROP_ELEMENT_NODE;
      case P3DResultField::NODAL_DISPLACEMENT:
        return TpvProperty::PROP_NODE;
      default:
        return -1;
    }
  }

  AlgPlane GetCuttingPlane() const
  {
    return m_cutplane;
  }

  TpvProperty* GetResultProperty() const
  {
    return m_property;
  }

//TODO remove dependency on Extrusion
  void SetPropertyIndirection(TpvProperty* property);

  void SetRezone(int nsteps, int* rezone);
  void GetRezone(int* nsteps, int** rezone);

  TpvProperty* GetContourProperty() const;

  void UpdateGeometry();
  void RebuildSortManager();
  void UpdateTransparency();
  void UpdateColorScale();
  void UpdateProperty();
  void UpdateDisp();
  void UpdateExtrusion();
  void LoadClippingPlane();
  void UnloadClippingPlane();
  void RenderElemHandles (int vpx, int vpy, int vpw, int vph);

  TpvProperty* ComputeMaterialProperty (P3DModel* m);
  TpvProperty* ComputeResultProperty (P3DResult* res);

private:
  P3DFemBoundaryRenderer* m_boundary_rnd;
  P3DFemBoundaryRenderer* m_hidden_boundary_rnd;
  P3DFemBoundaryRenderer* m_hidden_cutplane_boundary_rnd;
  P3DFemMaterialOutlineRenderer* m_mat_outline_rnd;
  P3DFemOutlineRenderer* m_outline_rnd;
  P3DFemOutlineRenderer* m_undeformed_outline_rnd;
  P3DFemVolumeRenderer* m_volume_rnd;
  P3DSelectedElemRenderer* m_elemsel_rnd;
  P3DSelectedNodesRenderer* m_nodesel_rnd;
  P3DFemVirtualNodesRenderer* m_virtual_nodes_rnd;

  bool m_update_geometry;
  bool m_update_property;
  bool m_update_disp;
  bool m_update_colorscale; 
  bool m_update_transp;
  bool m_isolines;
  bool m_bdry_enabled;
  bool m_bdry_faces_enabled;
  bool m_bdry_face_smooth_enabled;
  bool m_hidden_enabled;
  bool m_hidden_faces_enabled;
  bool m_hidden_face_smooth_enabled;
  bool m_hidden_bdry_cutplane_enabled;
  bool m_volumetric_enabled;
  bool m_isosurfaces_enabled;
  bool m_is_result_loaded;
  bool m_is_smooth_result_loaded;
  FaceMode m_bdry_face_mode;
  FaceMode m_hidden_face_mode;
  MeshMode m_bdry_mesh_mode;
  MeshMode m_hidden_mesh_mode;
  
  TpvProperty* m_material_property;
  float m_bdry_opacity_factor;
  float m_hidden_opacity_factor;
  float m_opacity_scale;

  int  m_rezone_nsteps;
  int* m_rezone;
  int  m_rezone_curr_step;
  bool m_virtual_nodes_enabled;
  bool m_outline_enabled;
  bool m_mat_outline_enabled;

  bool m_bdry_mesh_enabled;
  bool m_hidden_mesh_enabled;
  P3DElemVis* m_elemvis;
  P3DElemSelection* m_elemsel;
  P3DNodeSelection* m_nodesel;
  bool m_extrusion_enabled;

  P3DResult* m_result;
  TpvProperty* m_property;
  P3DResult* m_smooth_result;
  TpvProperty* m_smooth_property;
  P3DResult* m_disp;
  TpvProperty* m_disp_property;
  float m_disp_factor;
  bool m_undeform_enabled;
  bool m_cutplane_enabled;
  bool m_bdry_cutplane_affected;
  bool m_hidden_cutplane_affected;
  bool m_volume_cutplane_affected;
  AlgPlane m_cutplane;
  SciColorScale* m_colorscale;

  int m_viewport[4];
  unsigned m_selection_mode;
  P3DModel*  m_model;

  bool m_transp_enabled;
  bool m_linearize_opacity;
};

#endif

