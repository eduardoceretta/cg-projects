
#ifndef P3D_EXTRUSION_H
#define P3D_EXTRUSION_H

#include <ds/array.h>
#include <pos3dlib/defines.h>

class P3DModel;

class POS3DLIB_API P3DExtrusion
{
  // Maps nodes of the extruded element types to original 2D types (n-to-1 map).
  static bool m_rev_inc_map_init;
  static int m_rev_inc_map[TopModel::ELEMENT_MAX][TOP_MAX_NUM_N];
  // Maps nodes of the original 2D element types to extruded types (1-to-n map).
  static bool m_inc_map_init;
  static int m_inc_map[TopModel::ELEMENT_MAX][TOP_MAX_NUM_N][TOP_MAX_NUM_E_N+1];
  // Maps nodes of the extruded element types to elevation in normalized Z coord space.
  static bool m_rev_dz_map_init;
  static float m_rev_dz_map[TopModel::ELEMENT_MAX][TOP_MAX_NUM_N];

  static int GetExtrudedElemType(int type_id);
  static void InitRevIncMap();
  static void InitIncMap();
  static void InitRevDZMap();

  P3DModel* m_model;
  TopModel* m_mesh;
  TopModel* m_emesh;
  int m_max_node_id;
  int m_max_elem_id;
  int m_max_nnodes;
  int m_max_nelem;
  int m_max_nelem_nodes;
  int* m_node_ind;
  int* m_elem_node_global_ind;
  int* m_elem_node_local_ind;

  DsArray<float> m_pos;
  DsArray<float> m_dpos;
  float m_dirx, m_diry, m_dirz, m_factor;
  float m_dx, m_dy, m_dz;

  void ComputeElemTypeOrder(TopModel* m, int t[TopModel::ELEMENT_MAX]);
  void BuildNodeIndirection();
  void BuildElemNodeIndirection();
  void UpdateExtrusionDisplacement();
  void SetDefaultExtrusion();
  void UpdateNodePositions();
  bool InsertElem(const TopElement& el);

public:
  P3DExtrusion();
  ~P3DExtrusion();

  void SetModel(P3DModel* m);
  void SetExtrusion(float dirx, float diry, float dirz, float factor);
  void GetExtrusion(float* dirx, float* diry, float* dirz, float* factor);

  void BuildMesh();

  TopModel* GetExtrudedMesh() { return m_emesh; }
  int* GetNodeIndirection() { return m_node_ind; }
  int* GetElemNodeGlobalIndirection() { return m_elem_node_global_ind; }
  int* GetElemNodeLocalIndirection() { return m_elem_node_local_ind; }
};

#endif