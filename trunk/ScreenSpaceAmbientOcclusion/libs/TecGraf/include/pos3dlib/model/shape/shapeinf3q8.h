/************************************************************************
  shapeinf3q8.h
  rodesp@tecgraf.puc-rio.br
*************************************************************************/

#ifndef P3D_SHAPE_INF3Q8_H
#define P3D_SHAPE_INF3Q8_H

#include <pos3dlib/defines.h>
#include "../shape.h"

/*************************************************************************
  class P3DShapeINF3Q8

  Represents the 12-node three-dimensional Serendipity isoparametric single 
  infinite element.

  For this element type, mapping function (geometric mapping) is different 
  from shape function (field mapping). Here, however, the mapping function 
  is used to interpolate field data as well. 
  The third natural coordinate (t) corresponds to the infinite mapping, 
  whereas the other two (r,s) correspond to the finite mapping.

  @verbatim
                      inf                                inf
                      .                                   .
                     .                                   .
                    .                                   .
                  n11. . . . . . . . . . . . . . . . . n10
                (-1,-1,0)                             /. 
                 / .                                 / .
                /  .                                /  .
               /   .                               /   .
              /    .                              /    .
             /     .                             /     .    
            /      .                            /      . 
           /       .                           /       . 
          /        .                          /        .
        n3----------------n6----------------n2         .
  (-1,-1,-1)       .  inf                    |         .  inf
         |         .  .                      |         .  .
         |         . .                       |         . .
         |         ..                        |         ..
         |        n8 . . . . . . . . . . . . | . . . .n9
         |       /                           |      (1,1,0)
        n7      /                           n5       /
         |     /                             |      /
         |    /                              |     /
         |   /                               |    / 
         |  /                                |   /
         | /                                 |  /
         |/                                  | /
        n0----------------n4-----------------n1
                                            (1,1,-1)
          ^ t  
         /
        /
       +------> r
       |
       |
       v s

  @endverbatim

  Ref.:
    Marques JMMC, Owen DRJ (1984) "Infinite Elements in Quasi-Static 
    Materially Nonlinear Problems", Computers & Structures, 18(4):739-751

  NOTE: A corrected version of some 2D and 3D element types was presented in:
    Kay S, Bettess P (1996) "Revised Mapping Functions for Three-Dimensional
    Serendipity Infinite Elements", Communications in Numerical Methods in 
    Engineering, Vol. 12, pp. 181 - 184

  Additional (related) refs.:
    Zienkiewicz 0C, Emson C, Bettess P (1983) "A Novel Boundary Infinite 
    Element", International Journal for Numerical Methods in Engineering, 
    Vol. 19, pp. 393-404

    Barbier C, Bettess P, Bettess JA (1992) "Automatic Generation of Mapping 
    Functions for Infinite Elements using REDUCE", J. Symbolic Computation,
    Vol. 14, 523-534

    Abdel-Fattah TT, Hodhod HA, Akl AY (2000) "A novel formulation of infinite
    elements for static analysis", Computers and Structures, Vol. 77, 
    pp. 371-379
**************************************************************************/
class POS3DLIB_API P3DShapeINF3Q8 : public P3DShape
{
public:
  typedef P3DShape SuperClass;

  P3DShapeINF3Q8();
  virtual ~P3DShapeINF3Q8();
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
  virtual void ShapeFunc(float* natcoord, float* val_out);
  virtual void DerivShapeRST(const FemNatCoord* coord, FemNatCoord* deriv_out);
  virtual bool Jacobian(const FemNatCoord* deriv_map, const FemCartCoord* coord, 
    double jac[3][3], double jac_inv[3][3], double* det_jac);

private:
  static float m_natcoords[P3DShape::MAX_NUM_NODES][3];

  virtual const TopElementClass* GetElemClass();
};

#endif