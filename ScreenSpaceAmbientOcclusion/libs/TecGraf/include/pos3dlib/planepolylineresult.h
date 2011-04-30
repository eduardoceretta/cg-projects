/*************************************************************
*************************************************************/

#ifndef P3D_PLANE_POLYLINE_RESULT_H
#define P3D_PLANE_POLYLINE_RESULT_H

#include <alg/vector.h>
#include <alg/plane.h>
#include <alg/matrix.h>
#include <topsview/geometry/array.h>
#include <pos3dlib/defines.h>

class TpvTriangleSet;

struct POS3DLIB_API P3DPlanePolylineResultData 
{
  enum PointType{
    POINT_OUT = 0,
    POINT_IN, 
    POINT_ENTER,
    POINT_LEAVE
  };

  float t;  // Distance in polyline.
  float s;  // Scalar.
  int type; // Informa se esta dentro ou fora da malha (0- Fora, 1-Dentro)
  P3DPlanePolylineResultData() {}
  P3DPlanePolylineResultData(float t, float s, int type) : s(s), t(t), type(type) {}
  P3DPlanePolylineResultData(const P3DPlanePolylineResultData& p) : s(p.s), t(p.t), type(p.type) {}
};

typedef DsArray<P3DPlanePolylineResultData> P3DPlanePolylineResultDataArray;

class POS3DLIB_API P3DPlanePolylineResult
{
public:

  P3DPlanePolylineResult ();

  ~P3DPlanePolylineResult ();

  void Compute (const AlgPlane& plane,
                TpvFloatArray* tpvPolyPoints3d, 
                TpvTriangleSet* tpvTriangleSet, 
                P3DPlanePolylineResultDataArray* result);

private:
  AlgVector GetCoordPlane(const AlgMatrix& invbasemtx, AlgVector pworld)
  {
    return invbasemtx.Transform(pworld);
  }

  AlgVector GetCoordWorld(const AlgMatrix& basemtx, AlgVector pplane)
  {
    return basemtx.Transform(pplane);
  }

  void Compute (AlgMatrix& invbasemtx,
                TpvFloatArray* tpvPolyPoints3d, 
                TpvTriangleSet* tpvTriangleSet, 
                P3DPlanePolylineResultDataArray* result);

  P3DPlanePolylineResultData GetPolylinePoint2DPlaneData (float point[2], 
                                                float t,
                                                int numTri, 
                                                const float* triPoints2d, 
                                                const float* triScalars);

  void MapToPlaneCoords (AlgMatrix& invbasemtx,
                         int numPolyPoints, const float* polyPoints3d, 
                         int numTriPoints, const float* triPoints3d, 
                         float* polyPoints2d, float* triPoints2d);
};



#endif
