/************************************************************************
  P3DShape class
  Represents a finite element shape function.
  rodesp@tecgraf.puc-rio.br
*************************************************************************/

#ifndef P3D_SHAPE_H
#define P3D_SHAPE_H

#include <pos3dlib/defines.h>
#include "gauss.h"
#include "collapsemap.h"

/******************************************************************
  class P3DShape
  Represents element geometry and shape function (e.g. T3, Q4, etc.)
  Provides methods for obtaining natural coordinates (r,s,t),
  nodal incidence maps, extrapolation from gauss to element nodes.
******************************************************************/
class POS3DLIB_API P3DShape
{
public:
  enum {
    MAX_NUM_NODES = 20
  };

  P3DShape();

  virtual ~P3DShape();

  /** 
    Gets the TopS element type (topology) associated to this shape. 
  */
  virtual int GetElemType() = 0;

  /** 
    Gets the dimension of the element shape.
    Returns: 1 (1D), 2 (2D) or 3 (3D)
  */
  virtual int GetDimension() = 0;

  /**
    Gets the number of nodes interpolated by the shape function.
  */
//TODO rename to GetNumShapeNodes()
  virtual int GetNumOfNodes() = 0;

  /**
    Gets the natural coordinates of the shape nodes.
  */
  virtual float* GetNatCoords() = 0;  

  /**
    Gets the default gauss integration type associated to this shape function.
  */
  virtual int GetDefaultGaussType() = 0;

  /**
    Gets the default gauss integration order associated to this shape function.
  */
  virtual void GetDefaultGaussOrder(int* nr, int* ns, int* nt) = 0;

  /**
    Gets the default P3DGauss object associated to this shape function. 
  */
  virtual P3DGauss* GetDefaultGauss() = 0;

  /**
    Returns transformation matrix to compute nodal responses from Gauss
    responses. This matrix extrapolates values defined in the gauss
    points of the element to its nodes.

    The dimensions of the matrix are:
      nrows = num_nodes
      ncols = num_gauss_points
  */  
  virtual void ComputeGaussToNodeMatrix(P3DGauss* gauss, float* tr_matrix_out);

  float* GetGaussToNodeMatrix(P3DGauss* gauss);

  /**
    Maps results at gauss points to the element shape nodes.
    Resequencing is applied on the mapped results.
  */
  virtual void MapGaussToNode(
    P3DGauss* gauss, int dim, float* gauss_res, float* node_res);


  /**
   * Copies results at gauss points to the nearest element shape nodes.
   * Resequencing is applied on the mapped results.
   * If the number of nodes is different from the number of gauss points,
   * then the gauss points will be interpolated to the intermediate positions.
   */
  virtual void CopyGaussToNearestNodes(P3DGauss* gauss, 
                                       int dim, 
                                       float* gauss_res, 
                                       float* node_res);

  /**  
    Returns the cartesian coordinates corresponding to a
    location given in element local coordinates.
    Parameters:
      natcoord Position in natural coordinates
      dim Field dimension (1 - scalar, > 1 vector)
      nodefield Nodal field values ((x1,y1,z1),...,(xn,yn,zn)). Number of nodes
        is equal to number of shape nodes (see GetNumShapeNodes()).
      field_out Output vector
  */
  void InterpolatePosition(
    float* natcoord, const float* nodepos, float* cartcoord)
  {
    cartcoord[0] = cartcoord[1] = cartcoord[2] = 0.0f;
    float shape[TOP_MAX_NUM_N]; 
    ShapeFunc(natcoord, shape);
    int nnodes = GetNumOfNodes();
    for (int i = 0; i < nnodes; i++) {
      cartcoord[0] += shape[i] * nodepos[3*i];
      cartcoord[1] += shape[i] * nodepos[3*i+1];
      cartcoord[2] += shape[i] * nodepos[3*i+2];
    }
  }

  void InterpolatePosition(
    TopModel* m, TopElement elem, float* natcoord, float* cartcoord)
  {
    int nnodes = m->GetNNodes(elem);
    assert(nnodes == GetNumOfNodes());
    float nodepos[TOP_MAX_NUM_N][3];
    for (int i = 0; i < nnodes; i++)
      m->GetPosition(
        m->GetNode(elem, i), &nodepos[i][0], &nodepos[i][1], &nodepos[i][2]);
    InterpolatePosition(natcoord, (float*) nodepos, cartcoord);
  }

  /**  
    Returns the value of a nodal field interpolated at a given
    position in element local coordinates.
    Parameters:
      natcoord Po
      dim Field dimension (1 - scalar, > 1 vector)
      nodefield Nodal field values ((x1,y1,z1),...,(xn,yn,zn)). Number of nodes
        is equal to number of shape nodes (see GetNumShapeNodes()).
      field_out Output vector
  */
  void InterpolateField(
    float* natcoord, int dim, const float* nodefield, float* field_out)
  {
    float shape[TOP_MAX_NUM_N]; 
    ShapeFunc(natcoord, shape);
    for (int i = 0; i < dim; i++)
      field_out[i] = 0.0f;
    int nnodes = GetNumOfNodes();
    for (int i = 0; i < nnodes; i++)
      for (int j = 0; j < dim; j++)
        field_out[j] += shape[i] * nodefield[dim*i + j];
  }

  /**
    Returns whether the given parametrical coordinates lie inside the element's 
    parametrical boundaries.
  */
//TODO change to abstract method. Implement for every shape.
//  virtual bool IsInElement(float* natcoord) = 0;
  virtual bool IsInElement(float* natcoord) { return false; }

  /**
    Computes the natural coordinates corresponding to the given cartesian coordinates.
    The number of nodes of the element must be equal to number of shape nodes.
    Parameters:
      coord Position in the cartesian coordinate system.
    Returns whether a valid result could be achieved.
  */
  bool CartCoordToNatCoord(TopModel* m, TopElement elem, const float* coord, float* natcoord)
  {
//TODO Use shell elements (?)
    // Deal with 2D planar elements by projecting onto plane (x,y,0)
    float coordd[3] = {coord[0], coord[1], coord[2]};
    if (m->GetDimension(elem) == 2)
      coordd[2] = 0.0f;

    int nnodes = m->GetNNodes(elem);
    assert(nnodes == GetNumOfNodes());
    float nodepos[TOP_MAX_NUM_N][3];
    for (int i = 0; i < nnodes; i++)
      m->GetPosition(
        m->GetNode(elem, i), &nodepos[i][0], &nodepos[i][1], &nodepos[i][2]);
    return CartCoordToNatCoord((float*) nodepos, coordd, natcoord);
  }

  /** Returns whether the shape supports collapses. */
  bool CanCollapse() 
  { 
    return m_collapse_map.IsValid();
  }

  /** Returns whether element is collapsed (i.e., collapse_id != -1). */
  bool IsCollapsed(int collapse_id)
  {
    return collapse_id != -1;
  }

  /** 
    Given the shape node ids, returns the respective topology and collapse ids.
    @see GetCollapseMap()
  */
  void ComputeTopologyId(int* node_ids, int* topology_id, int* collapse_id);

  /**
    Gets the map which transforms the local node ids of the shape into the local 
    node ids of the topology.
  */
  int* GetShapeToTopologyIncMap(int collapse_id)
  {
    return m_collapse_map.GetShapeToTopologyIncMap(collapse_id);
  }

  /**
    Gets the map which transforms the local node ids of the topology into the local 
    node ids of the shape.
  */
  int* GetTopologyToShapeIncMap(int collapse_id)
  {
    return m_collapse_map.GetTopologyToShapeIncMap(collapse_id);
  }

protected:
  virtual const TopElementClass* GetElemClass() = 0;

  void InitCollapseMap(int ncollapses, int max_collapse_size, 
    int* collapses, int* collapse_types, int* shape_to_topology, int* topology_to_shape);

  /**
    Natural coordinates (r,s,t).
  */
  struct FemNatCoord
  {
    double r;
    double s;
    double t;
    FemNatCoord(double r = 0.0, double s = 0.0, double t = 0.0) : r(r), s(s), t(t) {}
    FemNatCoord(const FemNatCoord& c) : r(c.r), s(c.s), t(c.t) {}
    friend FemNatCoord operator+(const FemNatCoord& v1, const FemNatCoord& v2) 
    { 
      return FemNatCoord(v1.r + v2.r, v1.s + v2.s, v1.t + v2.t); 
    }
    friend FemNatCoord operator-(const FemNatCoord& v1, const FemNatCoord& v2) 
    { 
      return FemNatCoord(v1.r - v2.r, v1.s - v2.s, v1.t - v2.t); 
    }
  };

  /**
    Cartesian coordinates (x,y,z).
  */
  struct FemCartCoord
  {
    double x;
    double y;
    double z;
    FemCartCoord(double x = 0.0, double y = 0.0, double z = 0.0) : x(x), y(y), z(z) {}
    FemCartCoord(const FemCartCoord& c) : x(c.x), y(c.y), z(c.z) {}
    friend FemCartCoord operator+(const FemCartCoord& v1, const FemCartCoord& v2) 
    { 
      return FemCartCoord(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z); 
    }
    friend FemCartCoord operator-(const FemCartCoord& v1, const FemCartCoord& v2) 
    { 
      return FemCartCoord(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z); 
    }
  };

  /**  
    Returns the shape interpolation functions 
    evaluated at a given point in local coordinates.
  */
  virtual void ShapeFunc(float* natcoord, float* shape) = 0;

  /**
    Returns the derivatives of interpolation functions evaluated at shape nodes 
    in local coordinate system.
  */
//TODO find a more appropriate name
  virtual void DerivShapeRST(const FemNatCoord* coord, FemNatCoord* deriv_out) = 0;

  /**
    Computes the natural coordinates corresponding to a given position in an element.
    The position is defined in the cartesian coordinate system.
    Parameters:
      nodepos Coordinates of the nodes of the element. Number of nodes is equal to
        number of shape nodes (see GetNumShapeNodes()).
      coord Position in the cartesian coordinate system.
    Returns whether a valid result could be achieved.
  */
  bool CartCoordToNatCoord(
    const float* nodepos, const float* coord, float* natcoord);

  /**
    Computes the Jacobian matrix (transpose), its inverse and determinant.
    Returns true if computation could be performed or false if any numerical 
    problem ocurred.
  */
//TODO det_inv and det_jac could be computed separately from jac
  virtual bool Jacobian(const FemNatCoord* deriv_map, const FemCartCoord* coord, 
    double jac[3][3], double jac_inv[3][3], double* det_jac) = 0;

  bool JacobianArea(
    int num_map_nodes, const FemNatCoord* deriv_map, const FemCartCoord* coord, 
    double jac[3][3], double jac_inv[3][3], double* det_jac);

  bool JacobianSolid(
    int num_map_nodes, const FemNatCoord* deriv_map, const FemCartCoord* coord, 
    double jac[3][3], double jac_inv[3][3], double* det_jac);


private:
  P3DShapeCollapseMap m_collapse_map;
};

#endif