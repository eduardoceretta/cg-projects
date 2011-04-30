// Point renderer
// rodesp@tecgraf.puc-rio.br
// Mar 2011

#ifndef TPV_SCI_POINT_RENDERER_H
#define TPV_SCI_POINT_RENDERER_H

#include <alg/vector.h>
#include <topsview/defines.h>

#include "renderer.h"

class PtcPointRenderer;
class PtcCPUPointRenderer;
class PtcCPUAlphaPointRenderer;
class PtcGPUPointRenderer;
class TpvGeometry;
class TpvPointSet;
class TpvPtcPointSet;
class UGLTexture;

class TPV_API TpvSciPointRenderer : public TpvRenderer
{
public: 
  TpvSciPointRenderer ();
  
  virtual ~TpvSciPointRenderer ();


  //IGNORED
  virtual void SetWireframeColor (float r, float g, float b) 
  {
  }
  
  virtual void GetWireframeColor (float *r, float *g, float *b) 
  { 
    *r = 1.0f;
    *g = 1.0f;
    *b = 1.0f;
  }
  
  virtual void SetWireframeThickness (float t)
  {
  }
  
  virtual float GetWireframeThickness ()
  {
    return 1.0f;
  }

  virtual void SetWireframeIntensityAttenuation (bool flag)
  {
  }

  virtual bool GetWireframeIntensityAttenuation () const
  {
    return 1.0f;
  }
  ////////////////////////////////////////////////////

  void SetGeometry (TpvGeometry* g);

  /**
   * Configures the use of geometry field.
   */
  void SetFieldEnabled (bool flag)
  {
    m_field_enabled = flag;
  }

  bool Init ();

  void SetColorScaleTexture (UGLTexture* texture);

  void SetPointScale (float scale);

  /**
   * Sets the current camera position and direction.
   *
   * The parameters set by this method may be replaced when 
   * automatic determination of view parameters is enabled 
   * (see SetAutoUpdateViewParameters()).
   */
  void SetCamera (const AlgVector & position, const AlgVector & direction);

  /**
   * Sets the current camera field of view.
   *
   * The parameter set by this method may be replaced when 
   * automatic determination of view parameters is enabled 
   * (see SetAutoUpdateViewParameters()).
   */
  void SetFOV (float fov);

  /**
   * Sets whether camera view parameters (position, direction and fov) will be
   * automatically updated from the current OpenGL state.
   *
   * If enabled, the parameters set by SetCamera() and SetFOV() methods are
   * replaced by the automatic parameters computed.
   *
   * Default: true
   */
  void SetAutoUpdateViewParameters (bool enabled);

  void SetOrthoEnabled (bool enabled);

  void SetViewport (int width, int height);

  /**
   * Sets wheter volumetric rendering mode is enabled.
   * Default: false
   */
  void SetVolumetricEnabled (bool enabled);

  /**
   * Sets whether transparent rendering for points is enabled.
   * If transparency is not enabled, the alpha component of the 
   * colorscale is ignored.
   * Disabling transparency improves rendering speed, since
   * transparent points must be visibility sorted before rendering.
   * Default: true
   */
  void SetTransparencyEnabled (bool enabled);

  void SetTexturePath (const char* path);

  void SetShaderPath (const char* path);
 
protected:
  //void PrepareSetForRender (TpvGeometrySet *set);
  //void PrepareSetForRenderSelection (TpvGeometrySet *set);
  //void RenderSet (TpvPointSet *set);

  //void RenderSetSelection (TpvPointSet *set);

  /**
   * Is geometry field enabled?
   */
  bool IsFieldEnabled () const
  {
    return m_field_enabled;
  }

private:
  virtual void RenderBegin ();
  virtual void RenderEnd ();
  virtual void DoRender ();
  virtual void DoRenderSelection ();

  bool InitRenderer ();
  void Update ();
  void UpdateRenderer ();
  void UpdateCurrentRenderer ();
  void UpdateCamera ();
  void PrepareSetForRender (TpvPointSet* set);
  bool IsCameraPositionChanging (const AlgVector& new_position) const;
  bool IsCameraDirectionChanging (const AlgVector& new_direction) const;
  bool IsCameraFovChanging (float new_fov) const;

private:
  TpvGeometry* m_geometry;
  TpvPtcPointSet* m_ptc_pointset;
  UGLTexture* m_colorscale_texture;
  PtcPointRenderer* m_renderer;
  PtcCPUPointRenderer* m_cpurenderer;
  PtcCPUAlphaPointRenderer* m_cpualpharenderer;
  PtcGPUPointRenderer* m_gpurenderer;
  AlgVector m_camera_pos;
  AlgVector m_camera_dir;
  float m_camera_fov;
  bool m_camera_ortho_enabled;
  int m_camera_viewport_w;
  int m_camera_viewport_h;
  bool m_field_enabled;
  bool m_volumetric_enabled;
  bool m_transparency_enabled;
  bool m_auto_update_view;
  bool m_update_camera;
  bool m_update_renderer;
  bool m_initialized;
  bool m_renderer_initialized;
};

#endif

