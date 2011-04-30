//* femvirtualnodes.h
// rodesp@tecgraf.puc-rio.br
// Tecgraf/PUC-Rio
// Apr 2010

#ifndef P3D_FEM_VIRTUAL_NODES_RENDERER
#define P3D_FEM_VIRTUAL_NODES_RENDERER

#include <pos3dlib/defines.h>
#include "color.h"

class P3DModel;

/**
  Renders virtual nodes in a model.
*/
class POS3DLIB_API P3DFemVirtualNodesRenderer
{
public:
  P3DFemVirtualNodesRenderer ();
  ~P3DFemVirtualNodesRenderer ();
  void SetModel (P3DModel* model);
  void SetColor (float r, float g, float b, float a);
  void SetSize(float size);
  void Render ();
  void RenderSelection ();

private:
  P3DModel* m_model;
  P3DColor m_color;
  float m_size;
};

#endif
