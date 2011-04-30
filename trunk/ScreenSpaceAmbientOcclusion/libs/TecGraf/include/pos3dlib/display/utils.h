// Display utilities
// rodesp@tecgraf.puc-rio.br
// Tecgraf/PUC-Rio
// Apr 2010

#ifndef P3D_DISPLAY_UTILS_H
#define P3D_DISPLAY_UTILS_H

#include <topsview/property/property.h>
#include <topsview/renderer/renderer.h>

class P3DDisplayUtils
{
public:
  static TpvRenderer::PropMode GetTpvRendererPropMode (TpvProperty* prop);
};

#endif