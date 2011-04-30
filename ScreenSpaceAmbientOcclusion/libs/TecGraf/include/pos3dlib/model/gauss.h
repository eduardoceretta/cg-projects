/************************************************************************
  P3DModel class
  Represents a finite element Gauss integration.
  rodesp@tecgraf.puc-rio.br
*************************************************************************/

#ifndef P3D_GAUSS_H
#define P3D_GAUSS_H

#include <pos3dlib/defines.h>

/********************************************
  class P3DGauss
*********************************************/
class POS3DLIB_API P3DGauss
{
public:
  enum {
    MAX_NUM_POINTS = 27,
    MAX_ORDER = 4,
  };
  enum Type {
    QUADRATURE_INVALID = -1,
    QUADRATURE_TRI =0,
    QUADRATURE_QUAD,
    QUADRATURE_TETR,
    QUADRATURE_CUBE,
    QUADRATURE_NUM_TYPES
  };

private:
  int m_id;
  int m_type;
  int m_ns, m_nr, m_nt;
  int m_npoints;
  float m_points[MAX_NUM_POINTS][3];
  float m_weights[MAX_NUM_POINTS];

  void ComputePointsTri(int* npoints, 
    float gauss_rst[MAX_NUM_POINTS][3], float gauss_wgt[MAX_NUM_POINTS]);

  void ComputePointsQuad(int* npoints, 
    float gauss_rst[MAX_NUM_POINTS][3], float gauss_wgt[MAX_NUM_POINTS]);

  void ComputePointsTetr(int* npoints, 
    float gauss_rst[MAX_NUM_POINTS][3], float gauss_wgt[MAX_NUM_POINTS]);

  void ComputePointsCube(int* npoints, 
    float gauss_rst[MAX_NUM_POINTS][3], float gauss_wgt[MAX_NUM_POINTS]);

public:
  P3DGauss(int id, int type, int nr, int ns, int nt);

  virtual ~P3DGauss() {}

  void SetId(int id)
  {
    m_id = id;
  }

  /**
      Gets the id of this quadrature.
  */
  int GetId()
  {
    return m_id;
  }

  int GetType()
  {
    return m_type;
  }

  void GetOrder(int* nr, int* ns, int* nt)
  {
    *nr = m_nr; *ns = m_ns; *nt = m_nt;
  }

  int GetNumOfPoints()
  {
    return m_npoints;
  }

  float* GetNatCoords()
  {
    return (float*) m_points;
  }

  void GetNatCoord(int i, float* r, float* s, float* t) const
  {
    *r = m_points[i][0];
    *s = m_points[i][1];
    *t = m_points[i][2];
  }

  float* GetWeights()
  {
    return (float*) m_weights;
  }


  float GetWeight(int i) const
  {
    return m_weights[i];
  }

  int GetCubeIndex(int ri, int si, int ti)
  {
    return m_nr*m_ns*ti + m_nr*si + ri;
  }

  int GetQuadIndex(int ri, int si)
  {
    return m_nr*si + ri;
  }
};

#endif