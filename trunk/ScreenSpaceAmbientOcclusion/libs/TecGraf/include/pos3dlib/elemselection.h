//* elemselection.h
// Tecgraf/PUC-Rio
// rodesp@tecgraf.puc-rio.br
// favera@tecgraf.puc-rio.br
// Mar 2010
// Mar 2011

#ifndef P3D_ELEM_SELECTION_H
#define P3D_ELEM_SELECTION_H

#include <ds/array.h>
#include <pos3dlib/defines.h>
#include "model/model.h"


/**
 * class P3DElemSelection
 * Table of elements selected out of a P3DModel.
 */
class POS3DLIB_API P3DElemSelection
{
public:
  P3DElemSelection ();
  ~P3DElemSelection ();

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

  void AddSelectElems (bool *elements);
  void SelectElem (int id);

  void UnselectElem (int id);

  void Invert ();

  /**
    Returns the number of selected elements.
  */
  int GetNumSelected ();


  int GetSelectedElemIdAt (int index);

  /**
    Returns whether the element at global id 'gid' is selected.
  */
  bool IsSelected (int gid);

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

  DsArray<bool> m_elem_selected;
  
  DsArray<int> m_elem_selected_ids;

  bool m_outdated;
};


#endif