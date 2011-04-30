/************************************************************************
  shapecoh3q4.h
  rodesp@tecgraf.puc-rio.br
*************************************************************************/

#ifndef P3D_SHAPE_COH3Q4_H
#define P3D_SHAPE_COH3Q4_H

#include <pos3dlib/defines.h>
#include "../shape.h"

/********************************************
  class P3DShapeCOH3Q4
*********************************************/
class POS3DLIB_API P3DShapeCOH3Q4 : public P3DShape
{
public:
  typedef P3DShape SuperClass;

  P3DShapeCOH3Q4();
  virtual ~P3DShapeCOH3Q4();
  virtual int GetElemType();
  virtual int GetDimension();
  virtual int GetNumOfNodes();
  virtual float* GetNatCoords();
  virtual int GetDefaultGaussType();
  virtual void GetDefaultGaussOrder(int* nr, int* ns, int* nt);
  virtual P3DGauss* GetDefaultGauss();
  virtual void ComputeGaussToNodeMatrix(P3DGauss* gauss, float* tr_matrix_out);
  virtual void CopyGaussToNearestNodes(P3DGauss* gauss, 
                                       int dim, 
                                       float* gauss_res, 
                                       float* node_res);

protected:
  virtual void ShapeFunc(float* natcoord, float* val_out);
  virtual void DerivShapeRST(const FemNatCoord* coord, FemNatCoord* deriv_out);
  virtual bool Jacobian(const FemNatCoord* deriv_map, const FemCartCoord* coord, 
    double jac[3][3], double jac_inv[3][3], double* det_jac);

private:
  static float m_natcoords[P3DShape::MAX_NUM_NODES][3];

  virtual const TopElementClass* GetElemClass();
};

#endif