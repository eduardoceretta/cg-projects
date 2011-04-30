/**************************************************************
 **************************************************************/

#ifndef P3D_DISPLAY_H
#define P3D_DISPLAY_H

#include <pos3dlib/defines.h>
#include "color.h"

/*******************************************************************
  class P3DDisplay

    This class controls the displaying of a model on a given 
  subcanvas.
********************************************************************/
class POS3DLIB_API P3DDisplay
{
public:
  virtual ~P3DDisplay();
 
protected:
  explicit P3DDisplay();
};

#endif

