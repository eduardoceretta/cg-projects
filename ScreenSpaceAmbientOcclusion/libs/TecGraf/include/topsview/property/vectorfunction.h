#ifndef TPV_VECTORFUNCTION_H
#define TPV_VECTORFUNCTION_H

//----------//
// Includes //
//----------//

#include <alg/vector.h>
#include <tops/model.h>
#include <topsview/defines.h>

//----------------------//
// Forward Declarations //
//----------------------//

class TpvProperty;

//----------------------------------------------//
// Abstract Class Definition: TpvVectorFunction //
//----------------------------------------------//

class TPV_API TpvVectorFunction
{
public:

  enum InterpolationMode
  {
    INTERPOLATION_IDW         = 0,  // Use inverse distance weighting (hyperbolic non-homogeneous interpolation)
    INTERPOLATION_BARYCENTRIC = 1   // Use barycentric coordinates as weights (linear homogeneous interpolation)
  };

  // Constructors and Destructor

  TpvVectorFunction (TopModel *topModel = 0, unsigned int interpolation = 0);
  virtual ~TpvVectorFunction (void);

  // Getters and Setters

  TopModel*     GetTopModel       (void) {return m_topModel;}
  float         GetLimit          (void) {return m_limit;}
  unsigned int  GetInterpolation  (void) {return m_interpolation;}
  TopElement    GetCurrentElement (void) {return m_element;}
  TpvProperty* GetProperty       (void) {return m_property;}

  void SetTopModel       (TopModel*         topModel)      {m_topModel      = topModel;}
  void SetLimit          (float             limit)         {m_limit         = limit;}
  void SetInterpolation  (unsigned int      interpolation) {m_interpolation = interpolation;}
  void SetCurrentElement (const TopElement& element);

  bool SetProperty (TpvProperty* property, bool invertY = false, bool takePropertyOwnership = false);
  void SetProperty (const float* propertyX, const float* propertyY, const float* propertyZ, bool invertY = false, bool takeOwnership = false);
  void SetNullProperty (float nullX, float nullY, float nullZ);

  // Function Getter

  AlgVector GetFunctionValue (const AlgVector& spacePosition);

protected:

  // Auxiliary functions

  void ReleaseElementData (void);

  void LimitVector (AlgVector& vec);

  AlgVector GetNodeProperty (const TopNode& topNode);

  AlgVector GetFunctionValueIDW           (const AlgVector& spacePosition);
  AlgVector GetFunctionValueHex8          (const AlgVector& spacePosition);
  AlgVector GetFunctionValueHex8Reservoir (const AlgVector& spacePosition);
  AlgVector GetFunctionValueTetrahedron   (const AlgVector& spacePosition, int vertexId0, int vertexId1, int vertexId2, int vertexId3);

  // Member variables

  TopModel     *m_topModel;
  TpvProperty *m_property;

  const float *m_propertyX;
  const float *m_propertyY;
  const float *m_propertyZ;

  TopNode   *m_vertexNodes;
  AlgVector *m_vertexPos;

  AlgVector m_nullProperty;

  TopElement m_element;

  float m_limit;
  float m_signY;

  unsigned int m_interpolation;
  unsigned int m_numVertices;

  bool m_ownProperty;
  bool m_ownPropertyXYZ;

};

#endif // TPV_VECTORFUNCTION_H
