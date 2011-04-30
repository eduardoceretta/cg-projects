// Pos3D Render Engine Facade class
// rodesp@tecgraf.puc-rio.br
// May 2008

#ifndef P3D_RENDER_ENGINE_H
#define P3D_RENDER_ENGINE_H

class VglCanvas;
class VglSubCanvas;
class P3DModel;
class TopModel;
class LoadListener;
class P3DModel;
class P3DRenderEngineModel;
class P3DNfrModelReader;
class P3DFemDisplay;
class P3DResult;
class P3DElemVis;
class TpvColorScale;
class SciColorScale;
class SciActiveElementColorScale;
class P3DElemSelection;
class P3DElementSelector; 
class P3DMaterialSelector;
class SciActiveElementColorScaleIUP;
class P3DResultConverter;

#include <ds/array.h>
#include <vgl/handler.h>
#include <sciutils/actelem.h>
#include <pos3dlib/defines.h>

/**
    Listener for events generated during model reading.
*/
class POS3DLIB_API P3DRenderEngineLoadListener
{
public:
  virtual ~P3DRenderEngineLoadListener() {}

  /**
      Method invoked when a warning is reported during the reading.
  */
  virtual void WarningReported (const char* msg) {}

  /**
      Method invoked when an error is reported during the reading.
  */
  virtual void ErrorReported (const char* msg) {}

  /**
      Method invoked when the label of the current reading field
     has changed.
  */
  virtual bool ProgressLabelChanged (const char* msg) { return true; }

  /**
      Method invoked when the percentage ratio of the read data
    has changed.
  */
  virtual bool ProgressRatioChanged (double ratio) { return true; }
};

/**
  Model representation associated to a render engine.
  This class is a wrapper to the model representation used in Pos3D and
  allows the access to information related to the model itself, rather
  than the rendering context.
*/
class POS3DLIB_API P3DRenderEngineModel
{
public:
  enum ResType {
    RES_ELEMENT_GAUSS_SCALAR,
    RES_ELEMENT_NODAL_SCALAR,
    RES_NODAL_SCALAR,
  };
  
  ~P3DRenderEngineModel ();

  /**
    Gets the model's mesh. Returns a TopModel object.
  */
  TopModel* GetMesh ();

  /**
    Checks the consistency of the model's mesh. Returns true, if OK.
  */
  bool CheckConsistency (bool allow_non_manifold = false);

  /**
    Gets model's bounding box.
  */
  void GetBoundingBox (
    float* xmin, float* xmax, float* ymin, float* ymax, float* zmin, float* zmax);

  /**
    Gets the number of materials in the model.
  */
  int GetNumMaterials ();

  /**
    Gets the name of the i'th material.
  */
  const char* GetMaterialName (int i);

  int GetNumResCases ();
  const char* GetResCaseName (int casei);

  int GetNumResSteps (int casei);
  const char* GetResStepName (int casei, int stepi);

  int GetNumResFields (int casei, int stepi, ResType restype);
  const char* GetResFieldName (int casei, int stepi, ResType restype, int fieldi);

private:
  friend class P3DRenderEngine;

  P3DRenderEngineModel (P3DModel* model);

  P3DModel* GetModel () 
  {
    return m_model;
  }

  P3DModel* m_model;
};

/**
  Visual elements that are rendered on the screen, in front of the model.
  Currently, it only includes a color scale.
  The basic steps for using it are as follows:
  1 - Get the active elements object from the current render engine (see
  P3DRenderEngine::GetActiveElems()).
  2 - Set the canvas (or subcanvas) where the elements are going to be rendered.
  3 - Get the active element handler (see GetHandler()), add it
  in the first position of the handler list on the canvas (or subcanvas) and 
  activate the handler (handler->SetActive(true)).
  4 - Add an invocation to Reshape() method in the reshape function
  of the canvas (or subcanvas).
  5 - Add an invocation to Render() method in the display function
  of the canvas (or subcanvas), after the invocation to P3DRenderEngine::Render().
*/
class POS3DLIB_API P3DRenderEngineActiveElems
{
public:  
  ~P3DRenderEngineActiveElems ();

  /**
    Sets the canvas where the active elements will be
    rendered.
    Note: Replaces the current canvas or subcanvas, if present.
  */
  void SetCanvas (VglCanvas* canvas);

  /**
    Sets the subcanvas where the active elements will be
    rendered.
    Note: Replaces the current canvas or subcanvas, if present.
  */
  void SetSubCanvas (VglSubCanvas* subcanvas);

  /**
    Gets the VglHandler which manages the rendering and
    manipulation of the active elements.
  */
  SciActiveElementGroupHandler* GetHandler ();

  /**
    Ajusts the shape and position of the active elements.
  */
  void Reshape ();

  /**
    Renders active elements.
  */
  void Render ();

protected:
  P3DRenderEngineActiveElems ();  

  void SetColorScale (SciColorScale* colorscale);

  void SetColorScaleVisible (bool flag);

  void SetColorScaleNumberFormat (const char* format);

  void SetBackgroundColor (float r, float g, float b);

private:
  friend class P3DRenderEngine;

  VglCanvas* m_canvas;
  VglSubCanvas* m_subcanvas;
  SciActiveElementColorScale* m_actelem_colorscale;
  SciActiveElementGroup* m_actelem_group;
  SciActiveElementGroupHandler* m_actelem_group_hnd;
  SciColorScale* m_colorscale;
};

/**
  Abstraction for the render engine used in Pos3D.
  The basic steps for using the render engine are:
    1 - Load a model from a file (see LoadModel()).
    2 - Configure render settings:
      2.1 - Set render options: render mode, mesh attributes, etc. 
      (see SetRenderMode(), SetMeshEnabled(), etc.)
      2.2 - Set current result (see SetResult())
      2.3 - Set current color scale (see SetColorScale()), if it is the case 
      for the current render mode.
    3 - Call method Render() to draw the model. 
*/
class POS3DLIB_API P3DRenderEngine
{
public:
  enum ContourMode {
    /** Solid model rendering (no color scale is used). */
    CONTOUR_SOLID,
    /** Result contour */
    CONTOUR_RESULT,
    /** Material as colors rendering. */
    CONTOUR_MATERIAL,
    /** Per-element color rendering. */
    CONTOUR_PER_ELEMENT_COLORS
  };

  enum WireframeMode {
    /** Wireframe mesh on transparent boundary model faces. */
    WIREFRAME_TRANSP,
    /** Wireframe mesh on opaque boundary model faces. */
    WIREFRAME_OPAQUE
  };

  enum RenderMode {
    /** Renders the model in wireframe mode. */
    RENDER_WIREFRAME,
    /** Renders the model using contour. */
    RENDER_CONTOUR,
    /** Renders the model using volumetric mode. */
    RENDER_VOLUMETRIC,
  };

  enum SelectionMode {
    SELECT_NONE = 0,
    SELECT_ELEMENT = 1,
    SELECT_NODE = 2,
    //SELECT_FACE = 4,
  };

  P3DRenderEngine ();

  ~P3DRenderEngine ();

  /**
    Imports a text neutral (*.pos, *.nf, *.dat) into the binary format.
    Parameters:
      fname_txt: input file name (full path)
      fname_bin: name of the output binary file (full path)
      fname_idx: name of the output index file associated to the binary 
        one (full path).
  */
  static bool ImportNf (P3DRenderEngineLoadListener* l, 
    const char* fname_txt, const char* fname_bin, const char* fname_idx);

  /**
    Loads a model from a binary file into this render engine.
    Parameters:
      fname_bin: input binary file name (full path)
      fname_idx: input index file name (full path)
      l: listener for the events generated during model reading or NULL.
  */
  bool LoadModel (P3DRenderEngineLoadListener* l, 
    const char* fname_bin, const char* fname_idx);

  /**
    Loads a model from the given mesh.
    The mesh will be not destructed when render engine is destructed.
  */
  bool LoadModel (TopModel* mesh);

  /**
    Gets the model associated to this render engine.
  */
  P3DRenderEngineModel* GetModel ();

  /**
    Sets the current render mode.
  */
  void SetRenderMode (RenderMode mode);

  /**
    Sets the contour mode used when contour render mode is enabled.
  */
  void SetContourMode (ContourMode mode);

  /**
    Sets the mode used to display mesh when in wireframe render mode.
  */
  void SetWireframeMode (WireframeMode mode);

  /**
    Sets the color of the model's background.
    Used to render the model in the opaque wireframe mode.
  */
  void SetBackgroundColor (float r, float g, float b);

  /**
    Sets whether mesh visualization is enabled.
    Default: false
  */
  void SetMeshEnabled (bool flag);

  /**
    Sets the mesh color for bulk (solid) elements.
  */
  void SetMeshColor (float r, float g, float b);

  /**
    Sets line thickness of the mesh for bulk (solid) elements.
  */
  void SetMeshThickness (int i);

  /**
    Sets the mesh color for cohesive (interface) elements.
  */
  void SetCohMeshColor (float r, float g, float b);

  /**
    Sets line thickness of the mesh for cohesive (interface) elements.
  */
  void SetCohMeshThickness (int i);

  /**
    Sets whether node visualization is enabled.
  */
  void SetNodesEnabled (bool flag);

  void SetNodesColor (float r, float g, float b);

  void SetCohNodesColor (float r, float g, float b);

  /**
    Sets whether outlining of the model's mesh is enabled.
    Default: false.
  */
  void SetOutlineEnabled (bool flag);

  /**
    Sets whether outlining of the materials of the model is enabled.
    Default: false.
  */
  void SetMaterialOutlineEnabled (bool flag);

  void SetElemNumbersEnabled (bool flag);

  void SetNodeNumbersEnabled (bool flag);

  void SetSmoothResultEnabled (bool flag);

  /**
    Sets whether the visualization of the deformed mesh is enabled.
    Default: false
  */
  void SetDeformedEnabled (bool flag);

  /**
    Sets the displacemente factor used in the visualization of the 
    deformed mesh.
    Default: 1.0f
  */
  void SetDeformFactor (float factor);

  /**
    Sets whether the undeformed mesh will be drawn (as outline)
    along with the deformed one, in case the deformed visualization
    is enabled.
  */
  void SetUndeformedEnabled (bool flag);

  /**
    Sets the current result used for contouring.
  */
  void SetResult (int casei, int stepi, 
    P3DRenderEngineModel::ResType restype, int fieldi);

  /**
    Sets whether global time step, rather than local, color scale limits 
    will be used in contour mode.
  */
  void SetGlobalScaleLimitsEnabled (bool flag);

  /**
    Sets the current color scale.
    Parameters:
      ncolors: number of colors.
      colors_rgba: array of colors, of size = 4*ncolors
  */
  void SetColorScale (int ncolors, float* colors_rgba);

  /**
    Sets whether the color scale visualization is enabled.
    Color scale will only be rendered if an appropriated contour mode
    is set.
    Default: true.
  */
  void SetColorScaleEnabled (bool flag);

  /**
    Sets the colors used for rendering of materials.
    Parameters:
      colors_rgba: array of colors, of size = 4*num_materials
  */
  void SetMaterialColors (float* colors_rgba);

  /**
    Sets the color used for rendering the model's bulk (solid) elements in 
    solid contour mode (no color scale is used).
  */
  void SetElemColor (float r, float g, float b);

  /**
    Sets the color used for rendering the model's cohesive (interface) 
    elements in solid contour mode (no color scale is used).
  */
  void SetCohElemColor (float r, float g, float b);

  /**
    Sets the colors used when per-element colors rendering mode is enabled.
    Parameters:
      colors_rgba: array of colors, size = 4*num_elements
  */
  void SetPerElemColors (float* colors_rgba);

  void SetCutPlane (float a, float b, float c, float d);

  void SetCutPlaneEnabled (bool flag);

  /**
    Sets whether transparent rendering for solid elements is enabled.
    If transparency is not enabled, the alpha component of the 
    element colors or from the color scale is ignored.
    Default: false.
  */
  void SetTransparencyEnabled (bool flag);

  /**
    Sets whether the element at 'id' is visible.
    The element will be rendered only if both element and corresponding 
    material visibility (see SetMaterialVisible()) are enabled.
    Default: all elements are visible.
  */
  void SetElemVisible (int id, bool flag);

  /**
    Sets whether the i'th material is visible.
    An element will be rendered only if both material and element
    visibility (see SetElemVisible()) are enabled.
    Default: all materials are visible.
  */
  void SetMaterialVisible (int i, bool flag);

  /**
    Gets the active elements associated to this render engine.
  */
  P3DRenderEngineActiveElems* GetActiveElems ();

  /**
    Gets the results associated to the node with the given id.
    The values corresponding to the results of the node regarding incident 
    solid and interface elements, if any, are returned at the "solid_data" 
    and "coh_data" arrays.
    The arrays must be allocated by the client application and their sizes 
    must large enough for holding the returned values.
    The information on whether the node has results regarding solid or interface
    elements are returned at the parameters "has_solid_data" and "has_coh_data".
    Result smoothing must be enabled (see SetSmoothResultEnabled()).
    Currently, only scalar results are supported.
    In case a result can be computed, this method returns true, and false otherwise.
  */
  bool GetResultAtNode (int id, 
    bool* has_solid_data, float* solid_data, bool* has_coh_data, float* coh_data);

  /**
    Gets the results associated to the nodes of the element with the given id.
    The nodal values are returned at the "data" array passed as a parameter. 
    The parameter data must be allocated by the client application and its size 
    must large enough for holding the returned values.
    Currently, only scalar results are supported.
    In case a result can be computed, this method returns true, and false otherwise.
  */
  bool GetResultAtElemNodes (int id, float* data);

  /**
    Computes the shortest path along the edges on the mesh surface between the first 
    and last nodes in "polyline_nids" passing by the intermediate nodes.
    All the nodes in "polyline_nids" must be vertex nodes (if mid-side nodes are
    present, the algorithm will fail and return false).
    Node ids are returned in "nids" and positions (x,y,z) are returned in "pos".
  */
  bool ComputeShortestPath (
    DsArray<int>* polyline_nids, DsArray<int>* nids, DsArray<float>* pos);

  /**
    Computes scalar results along a polyline lying on the cutting plane. 
    Note: All polyline vertices must lie on the cutting plane. A scalar 
    result is required to be active and the cutting plane must be enabled.
    Parameters:
      polyline_pos: polyline vertex positions (x,y,z). All vertices
      must lie on the cutting plane.
      dist: (output) distances with respect to the beginning of the polyline
      corresponding to the computed scalar results. The length of this array 
      may be different from "polyline_pos", but is the same as "data".
      data: (output) scalar results computed along the polyline. The length of
      this array may be different from "polyline_pos", but is the same as "data".
    Returns "true" if results could be computed.
  */
  bool ComputeResultOnCutPlane (
    DsArray<float>* polyline_pos, DsArray<float>* dist, DsArray<float>* data);

  /**
    Sets the mode used for selection of model entities.
  */
  void SetSelectionMode (SelectionMode mode);

  void SetShaderPath (const char* path);

  /**
    Sets the current viewport.
    Required for RENDER_VOLUMETRIC mode.
  */
  void SetViewport (int x0, int y0, int w, int h);

  /**
    Renders the model onto the current active OpenGL canvas
    (or VglCanvas or VglSubCanvas).
  */
  void Render ();

  /**
    Renders the model in selection mode.
  */
  void RenderSelection ();

private:
  void UnloadModel ();
  void UpdateColorScaleLimits ();
  void UpdateColorScale ();
  void UpdateResult ();
  void UpdateDisplacement ();
  void UpdateRenderMode ();
  P3DResult* GetP3DResult (int case_id, 
                           int type_id,
                           int field_id, 
                           int step_id);

private:
  LoadListener* m_load_listener;
  P3DModel* m_p3d_model;
  P3DRenderEngineModel* m_model;
  P3DNfrModelReader* m_model_reader;
  P3DFemDisplay* m_renderer;
  RenderMode m_render_mode;
  ContourMode m_contour_mode; 
  WireframeMode m_wireframe_mode;
  TpvColorScale* m_colorscale;
  P3DResult* m_result;
  P3DResult* m_smooth_result;
  P3DElemVis* m_elemvis;
  P3DElementSelector* m_elemselector;
  P3DMaterialSelector* m_materialselector;
  P3DElemSelection* m_elemsel;
  bool m_update_elemvis;
  bool m_global_scale_enabled;
  bool m_smooth_result_enabled;
  bool m_deform_enabled;
  int m_res_casei;
  int m_res_stepi;
  P3DRenderEngineModel::ResType m_res_type;
  int m_res_fieldi;
  DsArray<float> m_matcolors;
  bool m_colorscale_enabled;
  AlgPlane m_cutplane;
  bool m_cutplane_enabled;
  bool m_bdry_transparency_enabled;
  bool m_mesh_enabled;
  P3DResultConverter* m_result_converter;

  SciColorScale* m_sci_colorscale;
  SciColorScale* m_sci_matcolorscale;
  SciColorScale* m_sci_renderer_colorscale;
  P3DRenderEngineActiveElems* m_actelems;
};

#endif
