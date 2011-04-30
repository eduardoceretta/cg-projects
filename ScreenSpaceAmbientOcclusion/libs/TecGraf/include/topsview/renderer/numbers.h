// Element/node number renderer
// rodesp@tecgraf.puc-rio.br
// May 2008

#ifndef TPV_NUMBERS_RENDERER_H
#define TPV_NUMBERS_RENDERER_H

#include <ds/list.h>
#include <ufont/fontmanager.h>
#include <topsview/defines.h>

class TpvGeometry;
class TpvTriangleSet;
class TpvPointSet;

#include "renderer.h"


class TPV_API TpvNumbersRenderer : public TpvRenderer
{
public: 
  TpvNumbersRenderer ();
  virtual ~TpvNumbersRenderer ();

  void SetGeometry (TpvGeometry* g);

  virtual void SetWireframeColor (float r, float g, float b)
  {
  }
  
  virtual void GetWireframeColor (float *r, float *g, float *b)
  {
    *r = *g = *b = 1.0f;
  }
  
  virtual void SetWireframeThickness (float t)
  {
  }
  
  virtual float GetWireframeThickness ()
  {
    return 1.f;
  }

  virtual void SetWireframeIntensityAttenuation (bool flag)
  {
  }

  virtual bool GetWireframeIntensityAttenuation () const
  {
    return false;
  }

  //virtual void SetClippingPlane(float a, float b, float c, float d);
  //virtual void SetClippingPlaneEnabled(bool flag);

  //virtual void SetPolygonOffset(float factor, float units);
  //virtual void SetPolygonOffsetEnabled(bool flag);

  //void SetRenderPoints(bool flag);
  //void SetRenderFaces(bool flag);

  //void SetPointColor (float r, float g, float b, float a);
  //void SetFaceColor (float r, float g, float b, float a);
  //float* GetPointColor ();
  //float* GetFaceColor ();

  void SetFont(const char *family_name, int size, bool bold = false, bool italic = false);

  void SetColor (float r, float g, float b, float a);
  void GetColor (float* r, float* g, float* b, float* a);

//TODO A display list precisa ser recriada sempre que a geometria é atualizada.
//TODO No momento, isso é feito pelo método abaixo.
//TOFO Procurar uma solução melhor (usar um esquema de listeners?)
  void SetDisplayListOutDated ()
  {
    m_update_dl = true;
  }

protected:  
  virtual void DoRender ();
  virtual void RenderBegin ();
  virtual void RenderEnd ();

private:
  void Update();

  void RenderSet (TpvTriangleSet* set);
  void RenderSet (TpvPointSet* set);

  UFontManager* m_fontmanager;
  TpvGeometry* m_geometry;
  bool  m_update_dl;
  unsigned int m_face_dl;
  unsigned int m_node_dl;
  float m_color[4];
  //float m_clip_plane[4];
  //bool  m_clip_plane_enabled;
  //float m_poly_offset_factor;
  //float m_poly_offset_units;
  //bool  m_poly_offset_enabled;
};

#endif
