// Renderers base class
// fabraham@tecgraf.puc-rio.br
// rodesp@tecgraf.puc-rio.br
// Feb 2008

#ifndef TPV_RENDERER_H
#define TPV_RENDERER_H

#include <topsview/defines.h>

class TPV_API TpvRenderer
{
public: 
  enum PropMode
  {
    PER_NODE,
    PER_ELEM_NODE,
    PER_ELEM,
    NONE,
  };
  
  TpvRenderer ();
  
  virtual ~TpvRenderer ();
  
  void SetWireframe (bool flag)
  {
    m_wireframe = flag;
  }
  
  bool GetWireframe () const
  {
    return m_wireframe;
  }
  
  virtual void SetWireframeColor (float r, float g, float b) = 0;
  
  virtual void GetWireframeColor (float *r, float *g, float *b) = 0;
  
  virtual void SetWireframeThickness (float t) = 0;
  
  virtual float GetWireframeThickness () = 0;

  /**
   * Enables wireframe intensity attenuation according to distance to observer.
   */
  virtual void SetWireframeIntensityAttenuation (bool flag) = 0;

  /**
   * Obtains the wireframe intensity attenuation flag.
   */
  virtual bool GetWireframeIntensityAttenuation () const = 0;

  void SetFieldMode (PropMode mode)
  {
    m_prop_mode = mode;
  }
  
  PropMode GetFieldMode () const
  {
    return m_prop_mode;
  }
  
  void SetLightingEnabled (bool flag)
  {
    m_lighting = flag;
  }
  
  bool IsLightingEnabled () const
  {
    return m_lighting;
  }
  
#if 0
  virtual void SetClippingPlane (float a, float b, float c, float d);
  virtual void SetClippingPlaneEnabled (bool flag);
  virtual void SetPolygonOffset (float factor, float units);
  virtual void SetPolygonOffsetEnabled (bool flag);
  /** 
   * If color mask is disabled, geometries will be rendered to the
   * depth-buffer only.    
   */
  void SetColorMaskEnabled (bool flag);
  virtual bool Update ();
#endif

  void Render ();
  
  void RenderSelection ();
  
private:
  virtual void RenderBegin () = 0;
  virtual void RenderEnd () = 0;
  virtual void DoRender () = 0;
  virtual void DoRenderSelection ()
  {
  }
#if 0
  float m_clip_plane[4];
  bool m_clip_plane_enabled;
  float m_poly_offset_factor;
  float m_poly_offset_units;
  bool m_poly_offset_enabled;
  bool m_color_mask_enabled;
#endif
  PropMode m_prop_mode;
  bool m_wireframe;
  bool m_lighting;
};

#endif

