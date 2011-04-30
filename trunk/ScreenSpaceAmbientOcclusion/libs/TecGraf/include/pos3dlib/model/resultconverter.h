
#ifndef P3D_RESULT_CONVERTER_H
#define P3D_RESULT_CONVERTER_H

class P3DModel;
class P3DResult;
class GlobalResultLimitsCache;

class P3DResultConverter
{
public:
  enum GaussExtrapolation {
    GAUSS_EXTRAPOLATION_NONE = 0,
    GAUSS_EXTRAPOLATION_SHAPE_FUNCTION,
    GAUSS_EXTRAPOLATION_COPY_TO_NEAREST_NODES
  };

  P3DResultConverter ();

  ~P3DResultConverter ();

  void SetModel (P3DModel* model);

  void SetResult (P3DResult* result);

  void SetGaussExtrapolationMethod (GaussExtrapolation method);

  P3DResult* GetResult ();

  P3DResult* GetSmoothResult ();

  P3DResult* GetExtrapolatedResult ();

  P3DResult* GetSmoothExtrapolatedResult ();

  P3DResult* GetElementGaussResult ();

//TODO Those methods are temporarily here. Refactor this class in order that
//TODO they are moved to a better place.
  void GetGlobalSmoothResultLimits (float* vmin, float* vmax);
  void GetGlobalExtrapolatedResultLimits (float* vmin, float* vmax);
  void GetGlobalSmoothExtrapolatedResultLimits (float* vmin, float* vmax);
  void GetGlobalElementGaussResultLimits (float* vmin, float* vmax);

private:
  void SetOutdated ();
  void UpdateSmoothResult ();
  void UpdateExtrapolatedResult ();
  void UpdateSmoothExtrapolatedResult ();
  void UpdateElementGaussResult ();
  void ReleaseResult (P3DResult** result);
  void ReleaseResults ();

private:
  P3DModel* m_model;
  P3DResult* m_result;
  P3DResult* m_smooth_result;
  P3DResult* m_smooth_extrapolated_result;
  P3DResult* m_extrapolated_result;
  P3DResult* m_element_gauss_result;
  bool m_smooth_result_outdated;
  bool m_smooth_extrapolated_result_outdated;
  bool m_extrapolated_result_outdated;
  bool m_element_gauss_result_outdated;
  GaussExtrapolation m_gauss_extrapolation_method;

  GlobalResultLimitsCache* m_global_smooth_limits_cache;
  GlobalResultLimitsCache* m_global_extrapolated_limits_cache;
  GlobalResultLimitsCache* m_global_smooth_extrapolated_limits_cache;
  GlobalResultLimitsCache* m_global_element_gauss_limits_cache;
};

#endif