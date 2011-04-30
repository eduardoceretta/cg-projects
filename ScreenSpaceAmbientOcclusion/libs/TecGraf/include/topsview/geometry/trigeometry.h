/**
* Modulo de definicao da classe TpvGeometry, para ser usado como geometria extraida para a TopsView
*/

#ifndef TPV_TRI_GEOMETRY_H
#define TPV_TRI_GEOMETRY_H

#include <tops/tops.h>
#include <topsview/defines.h>

#include "geometry3.h"
#include "triangleset2.h"

#include <stdio.h>
#include <math.h>

/**  
*/
MODEL_CLASS_TEMPLATE
class TPV_API TpvTriGeometry3 : public TpvGeometry3<MODELCLASS>
{
public:
  virtual TpvTriangleSet2* GetTriangles(int step_id = TpvGeometry3<MODELCLASS>::NO_REZONE_STEP_ID) = 0;
};

#endif
