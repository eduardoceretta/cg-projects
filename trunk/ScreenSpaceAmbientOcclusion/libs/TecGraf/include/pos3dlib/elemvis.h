//* elemvis.h
// rodesp@tecgraf.puc-rio.br
// Tecgraf/PUC-Rio
// Aug 2006

#ifndef P3D_ELEM_VIS_H
#define P3D_ELEM_VIS_H

#include <ds/array.h>
#include <pos3dlib/defines.h>
#include "model/model.h"
#include "undostack.h"
#include <ds/array.h>
#include <pos3dlib/defines.h>
#include "model/model.h"
#include "undostack.h"

#define P3D_UNDO_STACK_SIZE 10

class P3DElemSelection;
/************************************************************************
* class P3DElemVis
************************************************************************/
class POS3DLIB_API P3DElemVis
{
public:
  P3DElemVis();
  ~P3DElemVis();

  void SetModel (P3DModel* m);
  P3DModel* GetModel () { return m_model; }

  void SetSelection (P3DElemSelection *elem_selection, bool visible = true);
  void UndoLastSelection (bool visible = true );
  bool IsUndoEnabled ();

  void SetAllElements (bool visible);
  
  int GetVisibleElementIdAt (int index);
  int GetNumVisibleElements ();
  
  int GetHiddenElementIdAt (int index);
  int GetNumHiddenElements ();

  bool* GetArray ();
  bool* GetComplementaryArray ();
private:
  void Update ();

private:
  P3DModel* m_model;
  int m_nelem;

  LimitedStack<DsArray<int>*> m_undo_stack;

  DsArray<int> m_hidden_elem_ids;
  DsArray<int> m_visible_elem_ids;

  DsArray<bool> m_visible_elem; // Output visibility array
  DsArray<bool> m_hidden_elem; // Output complementary visibility array
  bool m_outdated;
};

#endif