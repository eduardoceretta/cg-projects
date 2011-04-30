/**
* Modulo de definicao da classe TpvGeometry, para ser usado como geometria extraida para a TopsView
*/

#ifndef TPV_POINT_GEOMETRY_H
#define TPV_POINT_GEOMETRY_H

#include <tops/tops.h>
#include <topsview/defines.h>

#include "geometry3.h"
#include "pointset2.h"

#include <stdio.h>
#include <math.h>

/**  
*/
MODEL_CLASS_TEMPLATE
class TPV_API TpvPointGeometry : public TpvGeometry3<MODELCLASS>
{
public:
  virtual TpvPointSet2* GetPoints(int step_id = TpvGeometry3<MODELCLASS>::NO_REZONE_STEP_ID) = 0;
};

#endif
