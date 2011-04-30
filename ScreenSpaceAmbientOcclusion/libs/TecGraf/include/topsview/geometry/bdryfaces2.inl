
#include "bdryfaces2.h"

MODEL_CLASS_TEMPLATE
void TpvBdryFacesGeometry2<MODELCLASS>::InitFaces()
{
  m_nsteps_alloc = GetRezoneNumSteps() + 1;
  m_faces = (TpvFacetUseArray**) calloc(m_nsteps_alloc, sizeof(TpvFacetUseArray*));
  for (int i = 0; i < m_nsteps_alloc; i++)
    m_faces[i] = new TpvFacetUseArray();

  m_faces2d = (TpvElementArray**) calloc(m_nsteps_alloc, sizeof(TpvElementArray*));
  for (int i = 0; i < m_nsteps_alloc; i++)
    m_faces2d[i] = new TpvElementArray();
}

MODEL_CLASS_TEMPLATE
void TpvBdryFacesGeometry2<MODELCLASS>::ClearFaces()
{
  for (int i = 0; i < m_nsteps_alloc; i++) 
    if (m_faces[i])
      delete m_faces[i];
  free(m_faces);

  for (int i = 0; i < m_nsteps_alloc; i++) 
    if (m_faces2d[i])
      delete m_faces2d[i];
  free(m_faces2d);
}

MODEL_CLASS_TEMPLATE
void TpvBdryFacesGeometry2<MODELCLASS>::ExtractFaces()
{
  TopModel* model = GetModel();
  assert(model != NULL);

  for (TopModel::ElemItr itr(model); itr.IsValid(); itr.Next()) {
    TopElement el = itr.GetCurr();

    if (!IsVisible(el))
      continue;

    int resid = GetElemRezoneId(el);
    if (model->GetDimension(el) == 2) {
      //if (!model->IsCohesive(el) && !model->IsInfinite(el))
      if (!model->IsCohesive(el))
        m_faces2d[resid]->Append(el);        
    } else {
      for (int fi = 0; fi < model->GetNFacets(el); fi++) {
        if (IsBoundary(el, fi))
          m_faces[resid]->Append(model->GetFacetUse(el, fi));
      }
    }
  }
}

/*************
public
**************/
//constructor
MODEL_CLASS_TEMPLATE
TpvBdryFacesGeometry2<MODELCLASS>::TpvBdryFacesGeometry2() : 
  m_update_geom(true),
  m_nsteps_alloc(0)
{
  InitFaces();
}

MODEL_CLASS_TEMPLATE
TpvBdryFacesGeometry2<MODELCLASS>::~TpvBdryFacesGeometry2() 
{
  ClearFaces();
}

MODEL_CLASS_TEMPLATE
void TpvBdryFacesGeometry2<MODELCLASS>::SetModel(TopModel* m)
{
  SuperClass::SetModel(m);
  m_update_geom = true;
}

MODEL_CLASS_TEMPLATE
void TpvBdryFacesGeometry2<MODELCLASS>::SetElemTypeVis(int type, bool isvisible)
{
  SuperClass::SetElemTypeVis(type, isvisible);
  m_update_geom = true;
}

MODEL_CLASS_TEMPLATE
void TpvBdryFacesGeometry2<MODELCLASS>::SetElemVis(const bool* vis)
{
  SuperClass::SetElemVis(vis);
  m_update_geom = true;
}

MODEL_CLASS_TEMPLATE
void TpvBdryFacesGeometry2<MODELCLASS>::SetRezone(int nsteps, int* rezone)
{
  SuperClass::SetRezone(nsteps, rezone);
  m_update_geom = true;
}

MODEL_CLASS_TEMPLATE
void TpvBdryFacesGeometry2<MODELCLASS>::SetBaseId(int id)
{
  SuperClass::SetBaseId(id);
  m_update_geom = true;
}

MODEL_CLASS_TEMPLATE
void TpvBdryFacesGeometry2<MODELCLASS>::Update()
{
  bool geom_changed = false;
  if (m_update_geom) {
    ClearFaces();
    InitFaces();
    ExtractFaces();
    geom_changed = true;
  }
  m_update_geom = false;
  if (geom_changed) 
    NotifyGeometryChanged();
}

MODEL_CLASS_TEMPLATE
int TpvBdryFacesGeometry2<MODELCLASS>::GetNumOfFaces(int step_id)
{
  return m_faces[GetStepRezoneId(step_id)]->Size();
}

MODEL_CLASS_TEMPLATE
TopFacetUse* TpvBdryFacesGeometry2<MODELCLASS>::GetFaces(int step_id)
{
  return m_faces[GetStepRezoneId(step_id)]->GetArray();
}

MODEL_CLASS_TEMPLATE
int TpvBdryFacesGeometry2<MODELCLASS>::GetNumOfFaces2D(int step_id)
{
  return m_faces2d[GetStepRezoneId(step_id)]->Size();
}

MODEL_CLASS_TEMPLATE
TopElement* TpvBdryFacesGeometry2<MODELCLASS>::GetFaces2D(int step_id)
{
  return m_faces2d[GetStepRezoneId(step_id)]->GetArray();
}

