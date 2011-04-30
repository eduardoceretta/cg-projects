
#ifndef P3D_ELE_NODE_WRITER_H
#define P3D_ELE_NODE_WRITER_H

#include <stdlib.h>
#include <ds/array.h>
#include <ds/hash.h>
#include <pos3dlib/defines.h>

class P3DModel;
class P3DResult;
class TopModel;

/**
   Writes the model mesh and an optional nodal scalar result to 
   a set of ASCII files using a simple file format (.ele/.node/.coh):
     ".ele": element incidences
     ".node": node positions
     ".coh": cohesive element incidences
 */
class POS3DLIB_API P3DEleNodeWriter
{
public:
  P3DEleNodeWriter (P3DModel* model, P3DResult* result = NULL);
  
  virtual ~P3DEleNodeWriter ();

  bool Write (const char* filename);

private:
  void ConstructOutMesh ();
  void ComputeOutField ();
  bool WriteMesh (const char* filename);
  bool WriteField (const char* filename);
  bool WriteAdjacencies (const char* filename);
  void ClearOutput ();

private:
  P3DModel* m_model;
  P3DResult* m_result;
  int m_out_node_base_id;
  int m_out_elem_base_id;
  bool m_write_field_with_nodes;
  bool m_write_adjacencies;
  DsHash<int, int> m_nodeid_to_out_nodeid;
  DsHash<int, int> m_elemid_to_out_elemid;
  TopModel* m_out_mesh;
  DsArray<float> m_out_field;

};


#endif