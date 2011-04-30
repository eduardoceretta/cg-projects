/**
*/

#ifndef TPV_POLY_GEOMETRY_H
#define TPV_POLY_GEOMETRY_H

#include <GL/glew.h>

#include <ds/array.h>
#include "geometry3.h"
#include "polygonset.h"
#include <topsview/defines.h>

#include <stdio.h>

/**
*/
MODEL_CLASS_TEMPLATE
class TPV_API TpvPolyGeometry: public TpvGeometry3<MODELCLASS>
{
public:
  virtual TpvPolygonSet* GetPolygons(int step_id = TpvGeometry3<MODELCLASS>::NO_REZONE_STEP_ID) = 0;
};
#endif

