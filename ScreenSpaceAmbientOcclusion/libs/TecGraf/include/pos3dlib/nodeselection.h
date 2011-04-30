//* nodeselection.h
// favera@tecgraf.puc-rio.br
// Tecgraf/PUC-Rio
// Mar 2010

#ifndef P3D_NODE_SELECTION_H
#define P3D_NODE_SELECTION_H

#include <ds/array.h>
#include <pos3dlib/defines.h>
#include "model/model.h"


/**
 * class P3NodeSelection
 * Table of nodes selected out of a P3DModel.
 */
class POS3DLIB_API P3DNodeSelection
{
public:
  P3DNodeSelection ();

  ~P3DNodeSelection ();

  /**
    Sets the model to which this selection is associated.
  */
  void SetModel (P3DModel* model);

  P3DModel* GetModel () 
  { 
    return m_model; 
  }

  /**
    Clears selection.
  */
  void Clear ();

  ///**
  //  Sets the default base id used to map from global element ids (gid's) in the mesh
  //  to positions of the selection flag array: pos = gid - baseid
  //  Default: 0
  //*/
  void SetBaseGlobalId (int gid)
  {
    m_base_gid = gid;
  }

  /**
    Sets selected nodes based on an array of selection flags.
  */
  //void SetSelection (bool* selection);

  void SetSelectedNode(int node_id, bool select_state);

  void SetSelectedVirtualNode(int node_id, bool select_state);

  /**
    Returns the number of selected nodes.
  */
  int GetNumSelectedNodes ()
  {
    return m_selected.Size();
  }
  
  /**
    Returns the number of selected virtual nodes.
  */
  int GetNumSelectedVirtualNodes ()
  {
    return m_selected_virtual.Size();
  }

  /**
    Returns the array of selected nodes
  */
  const DsArray<TopNode>* GetSelectedNodes ()
  {
    return &m_selected;
  }

  const DsArray<P3DVirtualNode*>* GetSelectedVirtualNodes ()
  {
    return &m_selected_virtual;
  }

  /**
    Returns whether the node at global id 'gid' is selected.
  */
  //bool IsSelected (int gid)
  //{
  //  return m_selection_flags.Get(gid - m_base_gid);
  //}

  /**
    Returns the array of selection flags.
    The size of the array is equal to the number of nodes in the mesh.
  */
//TODO rename
  const DsArray<bool>* GetSelectedNodesFlags ()
  {
    return &m_selection_flags;
  }
  /**
    Returns the array of selection flags.
    The size of the array is equal to the number of virtual nodes in the mesh.
  */
  const DsArray<bool>* GetSelectedVirtualNodesFlags ()
  {
    return &m_selection_virtual_flags;
  }

private:
  P3DModel* m_model;

  DsArray<TopNode> m_selected;
  DsArray<P3DVirtualNode*> m_selected_virtual;

  DsArray<bool> m_selection_flags;
  DsArray<bool> m_selection_virtual_flags;
  int m_base_gid;
};


#endif