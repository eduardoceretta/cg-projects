//* elementselector.h
// Tecgraf/PUC-Rio
// favera@tecgraf.puc-rio.br
// Mar 2011

#ifndef P3D_ELEMENT_SELECTOR_H
#define P3D_ELEMENT_SELECTOR_H

#include <ds/array.h>
#include <pos3dlib/defines.h>


class P3DElemSelection;
class P3DModel;

/**
 * class P3DElementSelector
 * Table of elements selected out of a P3DModel.
 */
class POS3DLIB_API P3DElementSelector
{
public:
  P3DElementSelector ();
  ~P3DElementSelector ();

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

  void SetInterfaceEnabled (bool f);
  bool GetInterfaceEnabled ();

  void SetSolidEnabled (bool f);
  bool GetSolidEnabled ();

  void SetElemIds (bool f);
  void SetElemIdRange (int minid, int maxid, bool f);
  void SetElemId (int id, bool f);

  void AddToSelection (P3DElemSelection *elemsel);
  void RemoveFromSelection (P3DElemSelection *elemsel);

  /**
    Returns the array of selection flags.
    The size of the array is equal to the number of elements in the mesh.
  */
  const DsArray<bool>* GetSelectionFlags ();

private:
  void Update();

private:
  P3DModel* m_model;
  int m_base_gid;

  bool m_interface;
  bool m_solid;
  int m_nelem;
  
  DsArray<bool> m_elem_selected;
  
  bool m_outdated;
};


#endif