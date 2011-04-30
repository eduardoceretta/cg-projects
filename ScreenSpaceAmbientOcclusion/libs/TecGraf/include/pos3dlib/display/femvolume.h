//* femvolume.h
// rodesp@tecgraf.puc-rio.br
// Tecgraf/PUC-Rio
// Apr 2010

#ifndef P3D_FEM_VOLUME_RENDERER_H
#define P3D_FEM_VOLUME_RENDERER_H

#include <alg/plane.h>
#include <ugl/texture.h>
#include <sciutils/colorscaletex.h>
#include <topsview/geometry/femtetrageometry3.h>
#include <topsview/renderer/harcvolrenderer3.h>
#include <pos3dlib/defines.h>

class TopModel;
class TpvProperty;
class TpvColorScale;
class TpvFemGeometryModel;
class SciColorScale;
class TpvElemPointsGeometry;
class TpvSciPointRenderer;
class P3DModel;
class P3DFemGaussPointsExtractor;

//TODO IsElemVisSupported()

class POS3DLIB_API P3DFemVolumeRenderer
{
public:
  P3DFemVolumeRenderer ();
  ~P3DFemVolumeRenderer ();
  bool Init ();
  void SetShaderPath (const char* path);
  void SetModel (P3DModel* model);
  void SetDisplacement (float factor, TpvProperty* displacement);
  void SetScalarField (TpvProperty* field);
  void SetColorScale (SciColorScale* colorscale, bool linearize_opacity);
  void SetOpacityFactor (float f);
  void SetNumSteps (int nsteps);
  void SetCurrentStep (int step);
  void SetVolumetricEnabled(bool enabled);

  /** Cutting plane */
  bool IsCuttingPlaneSupported ();
  void SetCuttingPlaneEnabled (bool flag);
  void SetCuttingPlane (AlgPlane plane);

  /** Probe box */
  bool IsProbeBoxSupported ();
  void SetProbeBoxEnabled (bool flag);
  void SetProbeBox (float xmin, float xmax, float ymin, float ymax,float zmin, float zmax);

  /** Isosurface */
  bool AreIsosurfacesSupported ();
  void SetIsosurfacesEnabled (bool flag);
  void SetIsosurfaceValuesEnabled (bool flag);
  void SetIsosurfaceValues (int n, const float* values);
  float GetMinNormalizedIsosurfaceValueInterval ();

  /** Gauss Points */
  void SetGaussPointsShaderPath(const char* path);
  void SetGaussPointsTexturePath(const char* path);
  bool AreGaussPointsSupported();
  void SetGaussPointsEnabled(bool flag);
  void SetGaussPointsScale (float scale);
  void SetGaussPointsVolumeRenderEnabled(bool flag);

  void SetViewport (int x0, int y0, int w, int h);
  void SetPolygonOffset (float factor, float units);
  bool Render ();

  /**********************
    Debug
  ***********************/
  void RenderDebugMesh ();

private:
  TpvColorScale* ComputeTpvColorScale (SciColorScale* scicolorscale);

  AlgMatrix GetTexLimitsTransform (float vmin, float vmax);

  void UpdateColorScale ();

  void LoadClippingPlane ();

  void UnloadClippingPlane ();

private:
  P3DModel* m_model;
  char* m_shader_path;
  SciColorScale* m_colorscale;
  TpvColorScale* m_tpv_colorscale;
  UGLTexture m_gauss_points_colorscale_tex;
  TpvTetraGeometry3<TpvFemGeometryModel>* m_tetra_geom;
  TpvElemPointsGeometry* m_gauss_points_geom;
  TpvHARCVolRenderer3<TpvFemGeometryModel>* m_volumetric_rnd;
  TpvSciPointRenderer* m_gauss_points_rnd;
  P3DFemGaussPointsExtractor* m_gauss_points_extractor;
  AlgPlane m_cutplane;
  float m_opacity_factor;
  bool m_volumetric_enabled;
  bool m_isosurfaces_enabled;
  bool m_gauss_points_enabled;
  bool m_probebox_enabled;
  bool m_cutplane_enabled;
  bool m_linearized_opacity;
  bool m_update_colorscale;
  bool m_initialized;
};

#endif