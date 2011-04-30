/************************************************************************
  shapet3.h
  rodesp@tecgraf.puc-rio.br
*************************************************************************/

#ifndef P3D_SHAPE_T3_H
#define P3D_SHAPE_T3_H

#include <pos3dlib/defines.h>
#include "../shape.h"

/********************************************
  class P3DShapeT3
  Linear triangular element.
*********************************************/
class POS3DLIB_API P3DShapeT3 : public P3DShape
{
public:
  typedef P3DShape SuperClass;

  P3DShapeT3();
  virtual ~P3DShapeT3();
  virtual int GetElemType();
  virtual int GetDimension();
  virtual int GetNumOfNodes();
  virtual float* GetNatCoords();
  virtual int GetDefaultGaussType();
  virtual void GetDefaultGaussOrder(int* nr, int* ns, int* nt);
  virtual P3DGauss* GetDefaultGauss();
  virtual void CopyGaussToNearestNodes(P3DGauss* gauss, 
                                       int dim, 
                                       float* gauss_res, 
                                       float* node_res);
  virtual bool IsInElement(float* natcoord);

protected:
  virtual void ShapeFunc(float* natcoord, float* shape);
  virtual void DerivShapeRST(const FemNatCoord* coord, FemNatCoord* deriv_out);
  virtual bool Jacobian(const FemNatCoord* deriv_map, const FemCartCoord* coord, 
    double jac[3][3], double jac_inv[3][3], double* det_jac);

private:
  static float m_natcoords[P3DShape::MAX_NUM_NODES][3];

  virtual const TopElementClass* GetElemClass();
};

#endif