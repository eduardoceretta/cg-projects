//* nodeselector.h
// Tecgraf/PUC-Rio
// favera@tecgraf.puc-rio.br
// Mar 2011

#ifndef P3D_NODE_SELECTOR_H
#define P3D_NODE_SELECTOR_H

#include <ds/array.h>
#include <pos3dlib/defines.h>

class P3DElemSelection;
class P3DModel;

/**
 * class P3DNodeSelector
 * Table of elements selected out of a P3DModel.
 */
class POS3DLIB_API P3DNodeSelector
{
public:
  P3DNodeSelector ();
  ~P3DNodeSelector ();

  /**
    Sets the model to which this selection is associated.
  */
  void SetModel (P3DModel* model);
  P3DModel* GetModel ();

  /**
    Clears selection.
  */
  void Clear ();

  /**
    Sets the default base id used to map from global element ids (gid's) in the mesh
    to positions of the selection flag array: pos = gid - baseid
    Default: 0
  */
  void SetBaseGlobalId (int gid);

  void SetNodeIds (bool f);
  void SetNodeIdRange (int minid, int maxid, bool f);
  void SetNodeId (int id, bool f);

  void AddToSelection (P3DElemSelection *elemsel);
  void RemoveFromSelection (P3DElemSelection *elemsel);
  
  /**
    Returns the array of selection flags.
    The size of the array is equal to the number of elements in the mesh.
  */
  const DsArray<bool>* GetSelectionFlags ();

private:
  void Update ();

private:
  P3DModel* m_model;
  int m_base_gid;

  int m_nelem;
  int m_nnodes;

  DsArray<bool> m_node_selected;
  DsArray<bool> m_elem_selected;
  
  bool m_outdated;
};


#endif