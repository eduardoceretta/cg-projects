/**
* Modulo de definicao da classe TpvGeometry, para ser usado como geometria extraida para a TopsView
*/

#ifndef TPV_BDRY_FACES_GEOMETRY_2_H
#define TPV_BDRY_FACES_GEOMETRY_2_H

#include <tops/tops.h>
#include <topsview/defines.h>

#include "facegeometry.h"

#include <stdio.h>
#include <math.h>

/**  
  class TpvGeometry3

    Represents a geometry extracted from a model.
  It contains polygonal and/or topological data from TopS.
*/
MODEL_CLASS_TEMPLATE
class TPV_API TpvBdryFacesGeometry2 : public TpvFaceGeometry<MODELCLASS>
{
private:
  bool m_update_geom;

  int m_nsteps_alloc;
  TpvFacetUseArray** m_faces;
  TpvElementArray** m_faces2d;

  bool IsVisible(const TopElement& el) const
  {
    assert(GetModel() != NULL);

    //return !GetModel()->IsInfinite(el) &&
    return IsElemTypeVisible(GetModel(), el) &&
           IsElemVisible(GetModel(), el);
  }

  bool IsBoundary(const TopElement& el, int fi) const
  {
    assert(GetModel() != NULL);

    TopElement adj = GetModel()->GetAdj(el, fi);
    if (GetModel()->IsCohesive(el) || !adj.IsValid() || !IsVisible(adj) ||
        GetElemRezoneId(el) != GetElemRezoneId(adj))
      return true;  
    else
      return false;
  }

  void InitFaces();
  void ClearFaces();
  void ExtractFaces();

public:
  typedef TpvFaceGeometry<MODELCLASS> SuperClass;

  TpvBdryFacesGeometry2();
  virtual ~TpvBdryFacesGeometry2();

  virtual void SetModel(TopModel* m);

  virtual void SetElemTypeVis(int type, bool isvisible);

  virtual void SetElemVis(const bool* vis);

  virtual void SetRezone(int nsteps, int* rezone);

  virtual void SetBaseId(int id);

  virtual void Update();

  virtual int GetNumOfFaces(int step_id = TpvGeometry3<MODELCLASS>::NO_REZONE_STEP_ID);
  virtual TopFacetUse* GetFaces(int step_id = TpvGeometry3<MODELCLASS>::NO_REZONE_STEP_ID);

  virtual int GetNumOfFaces2D(int step_id = TpvGeometry3<MODELCLASS>::NO_REZONE_STEP_ID);
  virtual TopElement* GetFaces2D(int step_id = TpvGeometry3<MODELCLASS>::NO_REZONE_STEP_ID);
};

#include "bdryfaces2.inl"

#endif
