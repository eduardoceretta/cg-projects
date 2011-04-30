/************************************************************************
  P3DModel class
  Represents a finite element model.
  rodesp@tecgraf.puc-rio.br
*************************************************************************/

#ifndef P3D_MODEL_H
#define P3D_MODEL_H

#include <ds/array.h>
#include <tops/tops.h>
#include <pos3dlib/defines.h>
#include "material.h"
#include "gauss.h"
#include "shape.h"
#include "resultcase.h"
#include "extrusion.h"
#include "virtualnode.h"

/**
  Listener called when the mesh consistency check finds an inconsistency.
*/
class POS3DLIB_API P3DModelConsistencyListener : public TopModelConsistencyListener
{
public:
  virtual ~P3DModelConsistencyListener() {}
  virtual bool ManifoldFailed(TopModel* m, TopNode n) { return false; }
  virtual bool CohesiveFailed(TopModel* m, TopElement coh) { return false; }
};

/**
  Listener called when an error is detected.
*/
class POS3DLIB_API P3DModelErrorListener : public TopErrorListener
{
public:
  virtual ~P3DModelErrorListener() {}
  virtual bool ErrorReported(const TopErrorHandler* e) { return false; }
};

/************************************************************************
 class P3DModel
 Represents a finite element model. The model consists of a mesh (TopModel)
 and analysis attributes (materials, results, etc.).
************************************************************************/
class POS3DLIB_API P3DModel
{
public:
  /**
    Creates a new empty model.
  */
  P3DModel();

  /**
    Creates a new model from a given mesh.
    The mesh will not be destructed when model is destructed.
  */
  P3DModel(TopModel* mesh);

  /**
    Destroys the model. All the associated data, as result cases and materials 
    are deleted.
  */
  ~P3DModel();

  /********************
    Mesh
  *********************/
  /**
    Gets the mesh of the model.
  */
  TopModel* GetMesh();

  /**
    Updates the model's bounding box.
  */
  void UpdateBoundingBox();

  /**
    Gets current model bounding box.anges in order to ensure that the correct bounding box is returned.
  */
  void GetBoundingBox(float* xmin, float* xmax, float* ymin, float* ymax,
      float* zmin, float* zmax);

  /**
    Computes bounding box of the deformed model.
  */
  void ComputeDeformedBoundingBox(P3DResultStep* res_step, float factor, 
      float* xmin, float* xmax, float* ymin, float* ymax,
      float* zmin, float* zmax);

  /**
    Gets an element's bounding box.
  */
  void GetElemBoundingBox(TopElement elem,
    float* xmin, float* xmax, float* ymin, float* ymax, float* zmin, float* zmax);

  /**
    Suggests a default deformation factor based on the model's bounding box.
  */
  float ComputeDefaultDeformFactor();

  /**
    Gets the number of elements in the model.
  */
  int GetNumOfElements();

  /**
    Gets the number of nodes in the model.
  */
  int GetNumOfNodes();

//TODO: otimizar!!! No momento, faz sempre a contagem.
  int GetNumOfFacets();
  int GetNumOfBdryFacets();
  int GetNumOfEdges();
  int GetNumOfBdryEdges();
  int GetNumOfVertices();
  int GetNumOfBdryVertices();

  /**
    Inserts a new node with the given id to the model.
  */
  TopNode InsertNode(int gid, float x, float y, float z);

  /**
    Gets the node with the given id. 
  */
  TopNode GetNode(int gid);

  int GetBaseNodeId() const
  {
    return 1;
  }

  /**
    Computes the deformed position of the node with the given id.
  */
  void ComputeDeformedPosition(int gid,
    P3DResultStep* res_step, float factor, float* x, float* y, float* z);

  /**
    Inserts a new element with the given id to the model.
  */
  TopElement InsertElem(int type, int gid, TopNode* nodes);

  /**
    Gets the element with the given id.
  */
  TopElement GetElem(int gid);

  int GetBaseElemId() const
  {
    return 1;
  }

  /**
    Gets an element which uses the node. The local id of the node 
    as used by the element is returned, too.

    The parameters bulk and coh defines that the elements from
    the respective type will be searched. If both are enabled, the
    solid element will be preferred.

    The parameter stepid defines the current result step. If it is 
    different from NULL, step dependent attributes, like rezone will be 
    considered.
  */
  TopElement GetElemNodeLocalId(TopNode node, bool bulk, bool coh, 
      P3DResultStep* res_step, int* lid_out);

  /********************
    Shape
  *********************/
  /**
    Adds a new shape to the model.
    @warning Model construction only.
  */
  void AddShape(P3DShape* s);

  /**
    Sets the array of shape ids of the elements of the model.
    array[elemid-1] = shapeid,  shapeid is 1-indexed
    @warning Model construction only.
  */
  void SetElemShapeIdArray(int* v);

  P3DShape* GetElemShape(int gid);

  /**
    Sets the array of shape collapse ids of the elements of the model.
    array[elemid-1] = collapseid,  collapseid is 0-indexed
    @warning Model construction only.
  */
  void SetElemCollapseIdArray(int* v);

  int GetElemCollapseId(int gid)
  {
    return m_elem_collapse_ids[gid-1];
  }

  /********************
    Material
  *********************/
  /**
    Adds a new material to the model.
    @warning Model construction only.
  */
  void AddMaterial(P3DMaterial* m);

  /**
    Returns the number of materials in the model.
  */
  int GetNumOfMaterials();

  /**
    Returns the i'th material. 
    Currently, it assumes that material id is equal to i+1.
  */
  P3DMaterial* GetMaterial(int i);

  /**
    Sets the array of material ids of the elements of the model.
    @warning Model construction only.
  */
  void SetElemMaterialIdArray(int* v);

  /**
    Gets the array of material ids of the elements of the model.
    array[elemid-1] = matid, matid is 1-indexed
  */
  int* GetElemMaterialIdArray();

  /** 
    Returns material id of the element with the given gid.
  */
  int GetElemMaterialId(int gid);

  int GetBaseMaterialId() const
  {
    return 1;
  }

  /********************
    Quadrature
  *********************/
  /**
    Adds a new gauss quadrature to the model.
    @warning Model construction only.
  */
  void AddQuadrature(P3DGauss* m);

  /**
    Returns the number of quadratures in the model.
  */
  int GetNumOfQuadratures();

  /**
    Returns the i'th quadrature. 
    Currently, it assumes that quadrature id is equal to i+1.
  */
  P3DGauss* GetQuadrature(int i);

  /**
    Sets the array of quadrature ids of the elements of the model.
    @warning Model construction only.
  */
  void SetElemQuadratureIdArray(int* v);

  /**
    Gets the array of quadrature ids of the elements of the model.
    array[elemid-1] = quadid, quadid is 1-indexed
  */
  int* GetElemQuadratureIdArray();

  /** 
    Returns quadrature id of the element with the given gid.
  */
  int GetElemQuadratureId(int gid);

  /**
    Returns the quadrature of the element with the given id.
   */
  P3DGauss* GetElemQuadratureById(int elem_id);

  /********************
    Rezone
  *********************/
  /**
    Array will be deleted when the model is destructed.
    @warning Model construction only.
  */
  void SetRezoneArray(int* v);

  /**
    Returns the array of rezone case id for each element id.
    v[id-1] = rezone_id.
  */
  int* GetRezoneArray();

  /**
    Returns the rezone id of the element with the given gid.
  */
  int GetElemRezoneId(int gid)
  {
    if (gid >= 1 && gid <= m_mesh->GetNElem() && m_mesh->GetElemAtId(gid).IsValid())
      return m_rezone[gid-1];
    else
      return 0;
  }

  /**
    Returns whether an element is rezoned
  */
  bool IsRezoned(int gid, P3DResultStep* res_step)
  {
    //Rezone id is 1-indexed
    int rezoneid = GetElemRezoneId(gid);
    return (rezoneid >= 1 && res_step && res_step->GetId() >= rezoneid);
  }

  /********************
    Result
  *********************/
  /**
    Sets total size (in number of floats) of an 1-dimensional 
    element Nodal result array for this model.
    @warning Model construction only.
  */
  void SetElemNodalTotalSize(int size);

  /**
    Gets total size (in number of floats) of an 1-dimensional 
    element nodal result array for this model.      
  */
  int GetElemNodalTotalSize();

  /** 
    Array will be deleted when the model is destructed.
    @warning Model construction only.
  */
  void SetElemNodalSizeArray(int* v);

  /**
    Returns the array of number of 1-dimensional element nodal results for each element id.
    v[id-1] = num_gauss_points.
  */
  int* GetElemNodalSizeArray();

  int GetElemNodalSize(int gid)
  {
    return GetElemNodalSizeArray()[gid-1];
  }

  /** 
    Array will be deleted when the model is destructed.
    @warning Model construction only.
  */
  void SetElemNodalOffsetArray(int* v);

  /**
    Returns the array of offsets (in number of floats) to element nodal 
    results for each element id.
    v[id-1] = offset to the first node of the element.
  */
  int* GetElemNodalOffsetArray();

  /** 
    Array will be deleted when the model is destructed.
    @warning Model construction only.
  */
  void SetElemGaussTotalSize(int size);

  /**
    Gets total size (in number of floats) of an 1-dimensional 
    element nodal result array for this model.      
  */
  int GetElemGaussTotalSize();

  /** 
    Array will be deleted when the model is destructed.
    @warning Model construction only.
  */
  void SetElemGaussSizeArray(int* v);

  /**
    Returns the array of number of gauss points for each element id.
    v[id-1] = num_gauss_points.
  */
  int* GetElemGaussSizeArray();

  /** 
    Array will be deleted when the model is destructed.
    @warning Model construction only.
  */
  void SetElemGaussOffsetArray(int* v);

  /**
    Returns the array of offsets to element gauss results for each element id.
    v[id-1] = offset to the first gauss point of the element.
  */
  int* GetElemGaussOffsetArray();  

  /**
    Gets the number of result cases in the model.
  */
  int GetNumOfResultCases();

  /**
    Gets the ith result case of the model.
  */
  P3DResultCase* GetResultCase(int i);

  /**
    Adds a result case to the model.
    @warning Model construction only.
  */
  void AddResultCase(P3DResultCase* res_case);

  /********************
    Extrusion
  *********************/
  /**
    Returns whether the model contains 2d elements or not.
  */
  bool HasElement2D();

  void SetExtrusion(float dirx, float diry, float dirz, float factor);

  void GetExtrusion(float* dirx, float* diry, float* dirz, float* factor);

  /**
    Builds the extruded mesh for 2d models. Must be called once before
    accessing the extruded mesh in the first time and after the model
    has been completely built (including results and ElemNodal and ElemGauss
    offset arrays).
  */
  void BuildExtrudedMesh();

  TopModel* GetExtrudedMesh();

  int* GetExtrudedNodeIndirection();

  /**
    Maps the extruded node id to the respective one in the original mesh.
  */
  int GetExtrudedNodeIndirectionAt(int extr_node_id);

  int* GetExtrudedElemNodeGlobalIndirection();

  int* GetExtrudedElemNodeLocalIndirection();

  /********************
    Virtual nodes
  *********************/
//TODO figure out the best way to keep virtual node list
  /**
    Returns the number of virtual nodes in the model.
  */
  int GetNumOfVirtualNodes();

  /**
    Returns the i'th virtual node. 
  */
  P3DVirtualNode* GetVirtualNode(int i);

  /**
    Adds a new virtual node to the model.
  */
  void AddVirtualNode(P3DVirtualNode* node);

  /**
    Removes the i'th virtual node.
    The following nodes are shifted one position back.
  */
  void RemoveVirtualNode(int i);

  /**
    Removes all the virtual nodes in the model.
  */
  void RemoveAllVirtualNodes();

  /********************
    Element location
  *********************/
  /**
    Locates the finite element corresponding to the given position
    in cartesian coordinates.
  */
  TopElement LocateElemAtPosition(
    float x, float y, float z, float* r, float* s, float* t);

  /********************
    Consistency
  *********************/
  bool CheckConsistency(P3DModelConsistencyListener* l = NULL,
                        bool allow_non_manifold = false);

  void SetErrorListener(P3DModelErrorListener* l);

private:
  static void InitShapes();

private:
  // Mesh
  TopModel*  m_mesh;
  bool       m_mesh_ownership;
  float      m_bbox[6];
  int        m_reseq[TopModel::ELEMENT_MAX][TOP_MAX_NUM_N];
  // Shapes
  int* m_elem_shapes;
  DsArray<P3DShape*> m_shapes;
  int* m_elem_collapse_ids;
  // Material
  int* m_elem_materials;
  DsArray<P3DMaterial*> m_materials;
  // Gauss
  int* m_elem_quadratures;
  DsArray<P3DGauss*> m_quadratures;
  // Rezone
  int* m_rezone;
  // Result
  int m_elem_nodal_size;
  int* m_elem_nodal_sizes;
  int* m_elem_nodal_offsets;
  int m_elem_gauss_size;
  int* m_elem_gauss_sizes;
  int* m_elem_gauss_offsets;
  DsArray<P3DResultCase*> m_res_cases;
  // Extrusion
  P3DExtrusion m_extrusion;
  // Virtual nodes
  DsArray<P3DVirtualNode*> m_virtual_nodes;
};

#endif