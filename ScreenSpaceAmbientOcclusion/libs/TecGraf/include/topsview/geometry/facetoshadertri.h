/**
* Modulo de definicao da classe TpvGeometry, para ser usado como geometria extraida para a TopsView
*/

#ifndef TPV_FACE_TO_SHADER_TRI_GEOMETRY_H
#define TPV_FACE_TO_SHADER_TRI_GEOMETRY_H

#include "facegeometry.h"
#include "trigeometry.h"
#include <topsview/defines.h>

#include <stdio.h>

/**  
*/
MODEL_CLASS_TEMPLATE class TPV_API TpvFaceToShaderTriGeometry : public TpvTriGeometry3<MODELCLASS>
{
private:
  TpvFaceGeometry<MODELCLASS>* m_face_geom;
  TpvProperty* m_field;
  TpvProperty* m_disp;
  float m_disp_factor;
  bool m_update_geom;
  bool m_update_disp;
  bool m_update_field;
  bool m_normalize_field;

  TpvTriangleSet2* m_triangles;
  TpvTriangleSet2* m_triangles_q4;
  TpvTriangleSet2* m_triangles_q8;
 
  void InitGeometry();
  void ClearGeometry();
  void ClearField();
  void ClearDisp();
  void AddTriangles(TpvTriangleSet2& triangles, const TopFacetUse& fu, bool extract_param);
  void AddTriangles2D(TpvTriangleSet2& triangles, const TopElement& el, bool extract_param);
  void ExtractGeometry();
  void AddTriangleField(TpvTriangleSet2& triangles, const TopFacetUse& fu);
  void AddTriangleFieldQ4(TpvTriangleSet2& triangles, const TopFaceUse& fu);
  void AddTriangleFieldQ8(TpvTriangleSet2& triangles, const TopFaceUse& fu);
  void AddTriangleField2D(TpvTriangleSet2& triangles, const TopElement& el);
  void AddTriangleFieldQ42D(TpvTriangleSet2& triangles, const TopElement& el);
  void AddTriangleFieldQ82D(TpvTriangleSet2& triangles, const TopElement& el);
  void ExtractField();

public:
  typedef TpvTriGeometry3<MODELCLASS> SuperClass;

  TpvFaceToShaderTriGeometry(TpvFaceGeometry<MODELCLASS>* g = NULL);
  virtual ~TpvFaceToShaderTriGeometry();

  virtual void SetFaceGeometry(TpvFaceGeometry<MODELCLASS>* g);
  virtual TpvFaceGeometry<MODELCLASS>* GetFaceGeometry();

  virtual void SetModel(TopModel* m);

  virtual void SetBaseId(int id);

  virtual void SetField(TpvProperty* f);
  virtual TpvProperty* GetField();

  virtual void SetDisp(TpvProperty* d);
  virtual TpvProperty* GetDisp();

  virtual void SetDispFactor(float f);
  virtual float GetDispFactor();

  virtual void Update();

  void SetNormalizeField(bool f);

  virtual TpvTriangleSet2* GetTriangles(int step_id = TpvGeometry3<MODELCLASS>::NO_REZONE_STEP_ID);
 
  TpvTriangleSet2* GetTrianglesQ4(int step_id = TpvGeometry3<MODELCLASS>::NO_REZONE_STEP_ID);

  TpvTriangleSet2* GetTrianglesQ8(int step_id = TpvGeometry3<MODELCLASS>::NO_REZONE_STEP_ID);
};

#include "facetoshadertri.inl"

#endif
