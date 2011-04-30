//* polyline.h
// rodesp@tecgraf.puc-rio.br
// Tecgraf/PUC-Rio
// Apr 2010

#ifndef P3D_POLYLINE_RENDERER_H
#define P3D_POLYLINE_RENDERER_H

#include <pos3dlib/defines.h>
#include <pos3dlib/polyline.h>

/**
  Renders a polyline in the three-dimensional space.
*/
class POS3DLIB_API P3DPolylineRenderer
{
public:
  P3DPolylineRenderer ();

  ~P3DPolylineRenderer ();

  void Render (const P3DPolyline* polyline);

private:
};

#endif