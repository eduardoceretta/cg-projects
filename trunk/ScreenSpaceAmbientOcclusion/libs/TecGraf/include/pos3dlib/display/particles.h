/**************************************************************
 **************************************************************/

#ifndef P3D_PARTICLES_DISPLAY_H
#define P3D_PARTICLES_DISPLAY_H

#include <vgl/vgl.h>
#include <sciutils/colorscale.h>
#include <topsview/colorscale/colorscale.h>
#include <topsview/property/property.h>
#include <particles/viewer.h>
#include <pos3dlib/defines.h>
#include "../model/model.h"
#include "../elemvis.h"
#include "../plane.h"

class PtcNFLoader;
class PtcViewer;
class PtcModel;
class UGLTexture;

#define P3D_PARTICLES_MAX_PATH PARTICLES_MAX_PATH

/*******************************************************************
  class P3DParticlesDisplay

    This class controls the displaying of a model on a given 
  subcanvas.
********************************************************************/
class POS3DLIB_API P3DParticlesDisplay
{
public:
  P3DParticlesDisplay ();

  ~P3DParticlesDisplay ();

  bool Init (const char* path);

  void SetShaderPath (const char* path);
  void SetTexturePath (const char* path);
  
  /** Current model. */
  void SetModel (PtcModel* model);
  PtcModel* GetModel ();

  /** Current model loader. */
  void SetLoader (PtcNFLoader* loader);
  PtcNFLoader* GetLoader ();

  /** Current colorscale. */
  TpvColorScale* GetColorScale ();
  /** Sets the current color scale. Creates a copy of the given one. */
  void SetColorScale (TpvColorScale* colorscale);
  /** deprecated */
  void SetColorScale (SciColorScale* colorscale);

  /**
    Sets the color used when no color scale is set.
  */
  void SetSolidColor (float r, float g, float b, float a);

  /**
  Sets the cutting plane.
  */
  void SetCuttingPlaneEnabled(bool flag);
  void SetCuttingPlane(float a, float b, float c, float d);
  void SetAffectedByCuttingPlane( bool flag );
  /**
  Sets whether model's transparent rendering is enabled.
  If transparency is not enabled, the alpha component of the 
  element colors or from the color scale is ignored.
  Default: false.
  */
  void SetTransparencyEnabled (bool flag);

  bool GetTransparencyEnabled () const
  {
    return m_transp_enabled;
  }

  /**
  Sets the rendering mode.
  Default: RENDERMODE_PARTICLES.
  */

  void SetRenderMode (PtcViewer::RenderMode mode);

  PtcViewer::RenderMode GetRenderMode () const
  {
    return m_rendermode;
  }

  /**
  Sets whether obstacle rendering is enabled.
  Default: false.
  */

  void SetObstaclesEnabled (bool flag);

  bool GetObstaclesEnabled () const
  {
    return m_obst_enabled;
  }


  /** Current view port. Must be updated whenever the view port changes. */
  void SetViewport (int x0, int y0, int w, int h);
  void GetViewport (int* x0, int* y0, int* w, int* h);

  /** Current model bounding box. */
  void SetBoundingBox (
    float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);
  void GetBoundingBox (
    float* xmin, float* xmax, float* ymin, float* ymax, float* zmin, float* zmax);

  /** Resets display to default values. */
  void Reset ();

  void Reshape (int w, int h);

  /** Render the model. */
  void Render ();

  /** Set the material colors. */
  void SetMaterialColors (SciColorScale* materials);

  void SetWireframeEnabled (bool flag);

  void SetOrtho (bool enable);

  void ResetParticlesMaterials (); 

private:
  bool InitParticles ();
 
private:
  char m_shader_path[P3D_PARTICLES_MAX_PATH+1];
  TpvColorScale* m_colorscale;
  PtcModel* m_model;
  PtcNFLoader* m_loader;
  PtcViewer* m_viewer;
  char m_texture_path[P3D_PARTICLES_MAX_PATH+1];
  bool m_transp_enabled;
  bool m_obst_enabled;
  PtcViewer::RenderMode m_rendermode;
  
  float* m_colorscale_tex_data;
  UGLTexture m_colorscale_tex;
  float m_color[4];
  DsArray<float> m_matcolors;

  int m_viewport[4];
  float m_bbox[6];
  int m_w, m_h;

  bool m_wireframe_enabled;
  bool m_cutplane_enabled;
  bool m_cutplane_affected;
  AlgPlane m_cutplane;
  bool m_colorscale_has_transparency;
};

#endif

