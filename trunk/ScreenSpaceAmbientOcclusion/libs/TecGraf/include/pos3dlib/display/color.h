//* color.h
// rodesp@tecgraf.puc-rio.br
// Tecgraf/PUC-Rio
// Apr 2010

#ifndef P3D_COLOR_H
#define P3D_COLOR_H

#include <pos3dlib/defines.h>

struct POS3DLIB_API P3DColor
{
  float r, g, b, a;
  P3DColor(float cr=0, float cg=0, float cb=0, float ca=1)
    : r(cr), g(cg), b(cb), a(ca) {}
};

#endif