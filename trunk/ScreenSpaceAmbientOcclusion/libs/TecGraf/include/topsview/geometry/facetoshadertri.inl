
#include "facetoshadertri.h"
#include "geometryutil.h"
#include "triangulator.h"

MODEL_CLASS_TEMPLATE
void TpvFaceToShaderTriGeometry<MODELCLASS>::InitGeometry()
{
  int nsteps, *rezone;
  m_face_geom->GetRezone(&nsteps, &rezone);
  TpvTriGeometry3<MODELCLASS>::SetRezone(nsteps, rezone);
  m_triangles = new TpvTriangleSet2[nsteps+1];
  m_triangles_q4 = new TpvTriangleSet2[nsteps+1];
  m_triangles_q8 = new TpvTriangleSet2[nsteps+1];
}

MODEL_CLASS_TEMPLATE
void TpvFaceToShaderTriGeometry<MODELCLASS>::ClearGeometry()
{
  if (m_triangles)
    delete[] m_triangles;
  m_triangles = NULL;
  if (m_triangles_q4)
    delete[] m_triangles_q4;
  m_triangles_q4 = NULL;
  if (m_triangles_q8)
    delete[] m_triangles_q8;
  m_triangles_q8 = NULL;
}

MODEL_CLASS_TEMPLATE
void TpvFaceToShaderTriGeometry<MODELCLASS>::ClearField()
{
  int nsteps = TpvTriGeometry3<MODELCLASS>::GetRezoneNumSteps();
  for (int i = 0; i < nsteps+1; i++) {
    m_triangles[i].GetField()->Resize(0);
    m_triangles_q4[i].GetField()->Resize(0);
    m_triangles_q8[i].GetField()->Resize(0);
  }
}

MODEL_CLASS_TEMPLATE
void TpvFaceToShaderTriGeometry<MODELCLASS>::ClearDisp()
{
  int nsteps = TpvTriGeometry3<MODELCLASS>::GetRezoneNumSteps();
  for (int i = 0; i < nsteps+1; i++) {
    m_triangles[i].SetSize(0);
    m_triangles[i].GetPositions()->Resize(0);
    m_triangles[i].GetNormals()->Resize(0);
    m_triangles_q4[i].SetSize(0);
    m_triangles_q4[i].GetPositions()->Resize(0);
    m_triangles_q4[i].GetNormals()->Resize(0);
    m_triangles_q8[i].SetSize(0);
    m_triangles_q8[i].GetPositions()->Resize(0);
    m_triangles_q8[i].GetNormals()->Resize(0);
  }
}

MODEL_CLASS_TEMPLATE
void TpvFaceToShaderTriGeometry<MODELCLASS>::AddTriangles(
    TpvTriangleSet2& triangles, const TopFacetUse& fu, bool extract_param)
{
  TopModel* model = TpvTriGeometry3<MODELCLASS>::GetModel();
  assert(model != NULL);

  float pos[3*TOP_MAX_NUM_F_N];
  TpvGeometryUtil::GetFacePositions(model, fu, m_disp, m_disp_factor, pos);
  int tri_size = triangles.GetSize();
  TpvFloatArray* tri_pos = triangles.GetPositions();
  TpvFloatArray* tri_normal = triangles.GetNormals();
  TpvSupportArray* tri_support = triangles.GetSupports();
  int el_type = model->GetElemType(fu);
  int fu_lid = model->GetLocalId(fu);
  int ntri = TpvTriangulator::GetFaceNumOfTriangles(el_type, fu_lid);   
  tri_pos->Resize(9*(tri_size + ntri));
  tri_normal->Resize(9*(tri_size + ntri));
  TpvTriangulator::TriangulateFaceGeometry(model, fu, pos, 
      &(tri_pos->GetArray()[9*tri_size]), &(tri_normal->GetArray()[9*tri_size]));
  if (extract_param) {
    TpvFloatArray* tri_param = triangles.GetParams();
    tri_param->Resize(6*(tri_size + ntri));
    TpvTriangulator::TriangulateFaceParam(model, fu, 
        &(tri_param->GetArray()[6*tri_size]));
  }
  // Extracts triangle support.
  TpvSupport support(TpvSupport::FACET_USE, fu.GetHandle());
  for (int i = 0; i < ntri; i++)
    tri_support->Append(support);
  triangles.SetSize(tri_size + ntri);
}

MODEL_CLASS_TEMPLATE
void TpvFaceToShaderTriGeometry<MODELCLASS>::AddTriangles2D(
    TpvTriangleSet2& triangles, const TopElement& el, bool extract_param)
{
  TopModel* model = TpvTriGeometry3<MODELCLASS>::GetModel();
  assert(model != NULL);

  float pos[3*TOP_MAX_NUM_F_N];
  TpvGeometryUtil::GetFace2DPositions(model, el, m_disp, m_disp_factor, pos);
  int tri_size = triangles.GetSize();
  TpvFloatArray* tri_pos = triangles.GetPositions();
  TpvFloatArray* tri_normal = triangles.GetNormals();
  TpvSupportArray* tri_support = triangles.GetSupports();
  int el_type = model->GetType(el);
  int ntri = TpvTriangulator::GetFace2DNumOfTriangles(el_type);   
  tri_pos->Resize(9*(tri_size + ntri));
  tri_normal->Resize(9*(tri_size + ntri));
  TpvTriangulator::TriangulateFace2DGeometry(model, el, pos, 
      &(tri_pos->GetArray()[9*tri_size]), &(tri_normal->GetArray()[9*tri_size]));
  if (extract_param) {
    TpvFloatArray* tri_param = triangles.GetParams();
    tri_param->Resize(6*(tri_size + ntri));
    TpvTriangulator::TriangulateFace2DParam(model, el, 
        &(tri_param->GetArray()[6*tri_size]));
  }
  // Extracts triangle support.
  TpvSupport support(TpvSupport::ELEMENT_2D, el.GetHandle());
  for (int i = 0; i < ntri; i++)
    tri_support->Append(support);
  triangles.SetSize(tri_size + ntri);
}

MODEL_CLASS_TEMPLATE
void TpvFaceToShaderTriGeometry<MODELCLASS>::ExtractGeometry()
{
  TopModel* model = TpvTriGeometry3<MODELCLASS>::GetModel();
  assert(model != NULL);

  int base_id = TpvTriGeometry3<MODELCLASS>::GetBaseId();
  for (int step_id = base_id + TpvTriGeometry3<MODELCLASS>::GetRezoneNumSteps();
       step_id >= base_id;
       step_id--)
  {
    int resid = TpvTriGeometry3<MODELCLASS>::GetStepRezoneId(step_id);
    // Faces
    int nfaces = m_face_geom->GetNumOfFaces(step_id);
    TopFacetUse* faces = m_face_geom->GetFaces(step_id);
    for (int fui = 0; fui < nfaces; fui++) {
      TopFacetUse fu = faces[fui];
      int el_type = model->GetElemType(fu);
      int fu_lid = model->GetLocalId(fu);
      switch (TpvGeometryUtil::GetElemFaceType(el_type, fu_lid)) {
        case TpvGeometryUtil::FACE_Q4:
          AddTriangles(m_triangles_q4[resid], fu, true);
          break;
        case TpvGeometryUtil::FACE_Q8:
          AddTriangles(m_triangles_q8[resid], fu, true);
          break;
        default:
          AddTriangles(m_triangles[resid], fu, false);
          break;
      }
    }
    // Faces 2D
    int nfaces2d = m_face_geom->GetNumOfFaces2D(step_id);
    TopElement* faces2d = m_face_geom->GetFaces2D(step_id);
    for (int fui = 0; fui < nfaces2d; fui++) {
      TopElement fu = faces2d[fui];
      int el_type = model->GetType(fu);
      switch (TpvGeometryUtil::GetElemFaceType(el_type, 0)) {
        case TpvGeometryUtil::FACE_Q4:
          AddTriangles2D(m_triangles_q4[resid], fu, true);
          break;
        case TpvGeometryUtil::FACE_Q8:
          AddTriangles2D(m_triangles_q8[resid], fu, true);
          break;
        default:
          AddTriangles2D(m_triangles[resid], fu, false);
          break;
      }
    }    
  }
}

MODEL_CLASS_TEMPLATE
void TpvFaceToShaderTriGeometry<MODELCLASS>::AddTriangleField(
  TpvTriangleSet2& triangles, const TopFacetUse& fu)
{
  TopModel* model = TpvTriGeometry3<MODELCLASS>::GetModel();
  assert(model != NULL);

  TpvFloatArray* tri_field = triangles.GetField();
  int tri_field_size = tri_field->Size();
  int el_type = model->GetElemType(fu);
  int fu_lid = model->GetLocalId(fu);
  int ntri = TpvTriangulator::GetFaceNumOfTriangles(el_type, fu_lid);
  float s[TOP_MAX_NUM_F_N];
  TpvGeometryUtil::GetFaceField(model, fu, m_field, s);
  tri_field->Resize(tri_field_size + 3*ntri);
  TpvTriangulator::TriangulateFaceField(model, fu, s, 
      &(tri_field->GetArray()[tri_field_size])); 
}

MODEL_CLASS_TEMPLATE
void TpvFaceToShaderTriGeometry<MODELCLASS>::AddTriangleFieldQ4(
  TpvTriangleSet2& triangles, const TopFaceUse& fu)
{
  TopModel* model = TpvTriGeometry3<MODELCLASS>::GetModel();
  assert(model != NULL);
  assert(model->GetNNodes(fu) == 4);
  TpvFloatArray* tri_field = triangles.GetField();
  int tri_field_size = tri_field->Size();
  int el_type = model->GetElemType(fu);
  int fu_lid = model->GetLocalId(fu);
  int ntri = TpvTriangulator::GetFaceNumOfTriangles(el_type, fu_lid);
  float s[TOP_MAX_NUM_F_N];
  if (m_field) {
    TpvGeometryUtil::GetFaceField(model, fu, m_field, s);
    if (m_normalize_field) {
      for (int i = 0; i < 4; i++)
        s[i] = m_field->NormalizeScalar(s[i]);
    }
  } else {
    for (int i = 0; i < 4; i++)
      s[i] = 0.0f;
  }
  tri_field->Resize(tri_field_size + 3*4*ntri);
  float* fp = &(tri_field->GetArray()[tri_field_size]);
  for (int i = 0; i < 3*ntri; i++) {
    for (int j = 0; j < 4; j++, fp++)
      *fp = s[j];
  }
}

MODEL_CLASS_TEMPLATE
void TpvFaceToShaderTriGeometry<MODELCLASS>::AddTriangleFieldQ8(
  TpvTriangleSet2& triangles, const TopFaceUse& fu)
{
  TopModel* model = TpvTriGeometry3<MODELCLASS>::GetModel();
  assert(model != NULL);
  assert(model->GetNNodes(fu) == 8);
  TpvFloatArray* tri_field = triangles.GetField();
  TpvFloatArray* tri_field2 = triangles.GetField2();
  int tri_field_size = tri_field->Size();
  int el_type = model->GetElemType(fu);
  int fu_lid = model->GetLocalId(fu);
  int ntri = TpvTriangulator::GetFaceNumOfTriangles(el_type, fu_lid);
  float s[TOP_MAX_NUM_F_N];
  if (m_field) {
    TpvGeometryUtil::GetFaceField(model, fu, m_field, s);
    if (m_normalize_field) {
      for (int i = 0; i < 8; i++)
        s[i] = m_field->NormalizeScalar(s[i]);
    }
  } else {
    for (int i = 0; i < 8; i++)
      s[i] = 0.0f;
  }
  tri_field->Resize(tri_field_size + 3*4*ntri);
  tri_field2->Resize(tri_field_size + 3*4*ntri);
  float* fp = &(tri_field->GetArray()[tri_field_size]);
  float* fp2 = &(tri_field2->GetArray()[tri_field_size]);
  for (int i = 0; i < 3*ntri; i++) {
    for (int j = 0; j < 4; j++, fp++, fp2++) {
      *fp = s[2*j];
      *fp2 = s[2*j+1];
    }
  }
}

MODEL_CLASS_TEMPLATE
void TpvFaceToShaderTriGeometry<MODELCLASS>::AddTriangleField2D(
  TpvTriangleSet2& triangles, const TopElement& el)
{
  TopModel* model = TpvTriGeometry3<MODELCLASS>::GetModel();
  assert(model != NULL);

  TpvFloatArray* tri_field = triangles.GetField();
  int tri_field_size = tri_field->Size();
  int el_type = model->GetType(el);
  int ntri = TpvTriangulator::GetFace2DNumOfTriangles(el_type);
  float s[TOP_MAX_NUM_F_N];
  TpvGeometryUtil::GetFace2DField(model, el, m_field, s);
  tri_field->Resize(tri_field_size + 3*ntri);
  TpvTriangulator::TriangulateFace2DField(model, el, s, 
      &(tri_field->GetArray()[tri_field_size])); 
}

MODEL_CLASS_TEMPLATE
void TpvFaceToShaderTriGeometry<MODELCLASS>::AddTriangleFieldQ42D(
  TpvTriangleSet2& triangles, const TopElement& el)
{
  TopModel* model = TpvTriGeometry3<MODELCLASS>::GetModel();
  assert(model != NULL);
  assert(model->GetNNodes(el) == 4);
  TpvFloatArray* tri_field = triangles.GetField();
  int tri_field_size = tri_field->Size();
  int el_type = model->GetType(el);
  int ntri = TpvTriangulator::GetFace2DNumOfTriangles(el_type);
  float s[TOP_MAX_NUM_F_N];
  if (m_field) {
    TpvGeometryUtil::GetFace2DField(model, el, m_field, s);
    if (m_normalize_field) {
      for (int i = 0; i < 4; i++)
        s[i] = m_field->NormalizeScalar(s[i]);
    }
  } else {
    for (int i = 0; i < 4; i++)
      s[i] = 0.0f;
  }
  tri_field->Resize(tri_field_size + 3*4*ntri);
  float* fp = &(tri_field->GetArray()[tri_field_size]);
  for (int i = 0; i < 3*ntri; i++) {
    for (int j = 0; j < 4; j++, fp++)
      *fp = s[j];
  }
}

MODEL_CLASS_TEMPLATE
void TpvFaceToShaderTriGeometry<MODELCLASS>::AddTriangleFieldQ82D(
  TpvTriangleSet2& triangles, const TopElement& el)
{
  TopModel* model = TpvTriGeometry3<MODELCLASS>::GetModel();
  assert(model != NULL);
  assert(model->GetNNodes(el) == 8);
  TpvFloatArray* tri_field = triangles.GetField();
  TpvFloatArray* tri_field2 = triangles.GetField2();
  int tri_field_size = tri_field->Size();
  int el_type = model->GetType(el);
  int ntri = TpvTriangulator::GetFace2DNumOfTriangles(el_type);
  float s[TOP_MAX_NUM_F_N];
  if (m_field) {
    TpvGeometryUtil::GetFace2DField(model, el, m_field, s);
    if (m_normalize_field) {
      for (int i = 0; i < 8; i++)
        s[i] = m_field->NormalizeScalar(s[i]);
    }
  } else {
    for (int i = 0; i < 8; i++)
      s[i] = 0.0f;
  }
  tri_field->Resize(tri_field_size + 3*4*ntri);
  tri_field2->Resize(tri_field_size + 3*4*ntri);
  float* fp = &(tri_field->GetArray()[tri_field_size]);
  float* fp2 = &(tri_field2->GetArray()[tri_field_size]);
  for (int i = 0; i < 3*ntri; i++) {
    for (int j = 0; j < 4; j++, fp++, fp2++) {
      *fp = s[j];
      *fp2 = s[4+j];
    }
  }
}

MODEL_CLASS_TEMPLATE
void TpvFaceToShaderTriGeometry<MODELCLASS>::ExtractField()
{
  TopModel* model = TpvTriGeometry3<MODELCLASS>::GetModel();
  assert(model != NULL);

  int base_id = TpvTriGeometry3<MODELCLASS>::GetBaseId();
  for (int step_id = base_id + TpvTriGeometry3<MODELCLASS>::GetRezoneNumSteps();
       step_id >= base_id;
       step_id--)
  {
    int resid = TpvTriGeometry3<MODELCLASS>::GetStepRezoneId(step_id);
    // Faces
    int nfaces = m_face_geom->GetNumOfFaces(step_id);
    TopFacetUse* faces = m_face_geom->GetFaces(step_id);
    for (int fui = 0; fui < nfaces; fui++) {
      TopFacetUse fu = faces[fui];
      int el_type = model->GetElemType(fu);
      int fu_lid = model->GetLocalId(fu);
      TopFaceUse fcu = model->GetFaceUse(model->GetElem(fu), fu_lid);
      switch (TpvGeometryUtil::GetElemFaceType(el_type, fu_lid)) {
        case TpvGeometryUtil::FACE_Q4:
          AddTriangleFieldQ4(m_triangles_q4[resid], fcu);
          break;
        case TpvGeometryUtil::FACE_Q8:
          AddTriangleFieldQ8(m_triangles_q8[resid], fcu);
          break;
        default:
          AddTriangleField(m_triangles[resid], fu);
          break;
      }
    }
    // Faces 2D
    int nfaces2d = m_face_geom->GetNumOfFaces2D(step_id);
    TopElement* faces2d = m_face_geom->GetFaces2D(step_id);
    for (int fui = 0; fui < nfaces2d; fui++) {
      TopElement fu = faces2d[fui];
      int el_type = model->GetType(fu);
      switch (TpvGeometryUtil::GetElemFaceType(el_type, 0)) {
        case TpvGeometryUtil::FACE_Q4:
          AddTriangleFieldQ42D(m_triangles_q4[resid], fu);
          break;
        case TpvGeometryUtil::FACE_Q8:
          AddTriangleFieldQ82D(m_triangles_q8[resid], fu);
          break;
        default:
          AddTriangleField2D(m_triangles[resid], fu);
          break;
      }
    }   
  }
}

/*************
public
**************/
//constructor
MODEL_CLASS_TEMPLATE
TpvFaceToShaderTriGeometry<MODELCLASS>::TpvFaceToShaderTriGeometry(TpvFaceGeometry<MODELCLASS>* g) : 
  m_face_geom(g),
  m_field(NULL), m_disp(NULL), m_disp_factor(1.0f),
  m_update_geom(true), m_update_disp(true), m_update_field(true),
  m_normalize_field(true),
  m_triangles(NULL), m_triangles_q4(NULL), m_triangles_q8(NULL)
{
  TpvGeometryUtil::Init();
  TpvTriangulator::Init();
  //InitGeometry();
}

MODEL_CLASS_TEMPLATE
TpvFaceToShaderTriGeometry<MODELCLASS>::~TpvFaceToShaderTriGeometry() 
{
  ClearGeometry();
}

MODEL_CLASS_TEMPLATE
void TpvFaceToShaderTriGeometry<MODELCLASS>::SetFaceGeometry(TpvFaceGeometry<MODELCLASS>* g)
{
  m_face_geom = g;
  m_update_geom = true;
}

MODEL_CLASS_TEMPLATE
TpvFaceGeometry<MODELCLASS>* TpvFaceToShaderTriGeometry<MODELCLASS>::GetFaceGeometry()
{
  return m_face_geom;
}

MODEL_CLASS_TEMPLATE
void TpvFaceToShaderTriGeometry<MODELCLASS>::SetModel(TopModel* m)
{
  SuperClass::SetModel(m);
  m_update_geom = true;
}

MODEL_CLASS_TEMPLATE
void TpvFaceToShaderTriGeometry<MODELCLASS>::SetBaseId(int id)
{
  SuperClass::SetBaseId(id);
  m_update_geom = true;
}

MODEL_CLASS_TEMPLATE
void TpvFaceToShaderTriGeometry<MODELCLASS>::SetField(TpvProperty* f)
{
  m_field = f;
  m_update_field = true;
}

MODEL_CLASS_TEMPLATE
TpvProperty* TpvFaceToShaderTriGeometry<MODELCLASS>::GetField()
{
  return m_field;
}

MODEL_CLASS_TEMPLATE
void TpvFaceToShaderTriGeometry<MODELCLASS>::SetDisp(TpvProperty* d)
{
  m_disp = d;
  m_update_disp = true;
}

MODEL_CLASS_TEMPLATE
TpvProperty* TpvFaceToShaderTriGeometry<MODELCLASS>::GetDisp()
{
  return m_disp;
}

MODEL_CLASS_TEMPLATE
void TpvFaceToShaderTriGeometry<MODELCLASS>::SetDispFactor(float f)
{
  m_disp_factor = f;
  m_update_disp = true;
}

MODEL_CLASS_TEMPLATE
float TpvFaceToShaderTriGeometry<MODELCLASS>::GetDispFactor()
{
  return m_disp_factor;
}

MODEL_CLASS_TEMPLATE
void TpvFaceToShaderTriGeometry<MODELCLASS>::Update()
{ 
  m_face_geom->Update();

  bool geom_changed = false;
  bool field_changed = false;
  if (m_update_geom) {
    ClearGeometry();
    InitGeometry();
    ExtractGeometry();
    // Displacement is updated in UpdateGeometry().
    m_update_disp = false;
    m_update_field = true;
    geom_changed = true;
  }
  if (m_update_disp) {
    ClearDisp();
    ExtractGeometry();
    geom_changed = true;
  }
  if (m_update_field) {
    ClearField();
    ExtractField();
    field_changed = true;
  }
  m_update_geom = false;
  m_update_disp = false;
  m_update_field = false;
  if (geom_changed)
    TpvTriGeometry3<MODELCLASS>::NotifyGeometryChanged();
  if (field_changed)
    TpvTriGeometry3<MODELCLASS>::NotifyFieldChanged();
}

MODEL_CLASS_TEMPLATE
void TpvFaceToShaderTriGeometry<MODELCLASS>::SetNormalizeField(bool f)
{
  m_normalize_field = f;
  m_update_field = true;
}

MODEL_CLASS_TEMPLATE
TpvTriangleSet2* TpvFaceToShaderTriGeometry<MODELCLASS>::GetTriangles(int step_id)
{
  return &m_triangles[TpvTriGeometry3<MODELCLASS>::GetStepRezoneId(step_id)];
}

MODEL_CLASS_TEMPLATE
TpvTriangleSet2* TpvFaceToShaderTriGeometry<MODELCLASS>::GetTrianglesQ4(int step_id)
{
  return &m_triangles_q4[TpvTriGeometry3<MODELCLASS>::GetStepRezoneId(step_id)];
}

MODEL_CLASS_TEMPLATE
TpvTriangleSet2* TpvFaceToShaderTriGeometry<MODELCLASS>::GetTrianglesQ8(int step_id)
{
  return &m_triangles_q8[TpvTriGeometry3<MODELCLASS>::GetStepRezoneId(step_id)];
}
