//* selectednodes.h
// favera@tecgraf.puc-rio.br
// Tecgraf/PUC-Rio
// Nov 2010

#ifndef P3D_SELECTED_NODES_RENDERER_H
#define P3D_SELECTED_NODES_RENDERER_H

class P3DModel;
class TopModel;
class P3DNodeSelection;
class TpvProperty;

#include <ds/array.h>
#include <pos3dlib/defines.h>

/**
  Renders points of nodes selected from a model.
*/
class POS3DLIB_API P3DSelectedNodesRenderer
{
public:
  P3DSelectedNodesRenderer ();
  ~P3DSelectedNodesRenderer ();

  void SetModel (P3DModel* model);
  void SetSelection (P3DNodeSelection* selection);
  void SetDisplacement (float factor, TpvProperty* displacement);
  //void SetPolygonOffset (float factor, float units);
  void SetColor(float r, float g, float b);
  void Render ();

  float GetNodeSize() const;
  void SetNodeSize(float val);

private:
  P3DModel* m_model;
  TopModel* m_mesh;
  const DsArray<bool>* m_selflags;
  const DsArray<bool>* m_sel_virtual_flags;
  P3DNodeSelection* m_nodesel;

  float m_points_color[3];
  float m_nodes_size;

  int m_base_gid;

  float m_displacement_factor;
  TpvProperty* m_displacement;
};

#endif