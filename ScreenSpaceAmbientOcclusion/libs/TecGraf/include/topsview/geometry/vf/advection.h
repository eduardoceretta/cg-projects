#ifndef TPV_ADVECTION_H
#define TPV_ADVECTION_H

//----------//
// Includes //
//----------//

#include <ds/array.h>
#include <alg/vector.h>
#include <vfl/advection.h>
#include <topsview/property/vectorfunction.h>
#include <topsview/geometry/vf/fieldline.h>
#include <topsview/defines.h>

//----------------------//
// Forward Declarations //
//----------------------//

class TopModel;
class TopElement;
class TpvVectorFunction;

//-------------------------------//
// Class Definition: TpvAdvector //
//-------------------------------//

class TPV_API TpvAdvector : public VflAdvector
{
public:

  // Constructors and Destructor

  TpvAdvector (
    TopModel     *topModel            = 0,
    unsigned int  integrationMethod   = INT_RUNGEKUTTA4,
    unsigned int  tangentMethod       = TAN_2POINT,
    unsigned int  normalMethod        = NORM_MAXREFLECTION);

  virtual ~TpvAdvector (void);

  // Getters and Setters

  TopModel*          GetTopModel          (void) {return m_topModel;}
  const bool*        GetActiveCells       (void) {return m_activeCells;}
  TpvVectorFunction* GetVectorFunction    (void) {return m_vectorFunction;}
  float              GetTimeStep          (void) {return m_timeStep;}
  unsigned int       GetNumStepsPerVertex (void) {return m_numStepsPerVertex;}
  unsigned int       GetMaxVertexCount    (void) {return m_maxVertexCount;}

  void SetTopModel       (TopModel *topModel)
  {
    m_topModel = topModel;
    m_topModelElemCount = m_topModel->GetNElem();
  }

  void SetActiveCells    (const bool* activeCells);
  void SetVectorFunction (TpvVectorFunction* vectorFunction, bool takeFunctionOwnership = false);

  void SetIntegrationParameters (float timeStep, unsigned int numStepsPerVertex, unsigned int maxVertexCount)
  {
    m_timeStep = timeStep;
    m_numStepsPerVertex = numStepsPerVertex;
    m_maxVertexCount = maxVertexCount;
  }

  void AddSeed    (const TopElement& seedElement, const AlgVector& seedPoint);
  void ClearSeeds (void);

  // Advection (computes every tangent as the field vector at each line vertex)

  void Advect (VflFieldLineList* fieldLineList)
  {
    m_topModelElemCount = m_topModel->GetNElem();
    if (IsUsingGpuAdvection()) AdvectGpu(fieldLineList);
    else AdvectCpu(fieldLineList);
  }

  VflMultiFieldLine* AdvectInterleaved (void)
  {
    m_topModelElemCount = m_topModel->GetNElem();
    if (IsUsingGpuAdvection()) return AdvectInterleavedGpu();
    else return AdvectInterleavedCpu();
  }

  //void AdvectRenderGpu (VflFieldLineRenderer *renderer);

protected:

  void AdvectCpu (VflFieldLineList* fieldLineList);
  void AdvectGpu (VflFieldLineList* fieldLineList);

  VflMultiFieldLine* AdvectInterleavedCpu (void);
  VflMultiFieldLine* AdvectInterleavedGpu (void);

  bool AdvectCpu (VflFieldLine *fieldLine, const TopElement &seedElement, const AlgVector& seedPoint);
  void AdvectGpu (VflFieldLine *fieldLine, const TopElement &seedElement, const AlgVector& seedPoint, unsigned int queryObject);

  bool AdvectInterleavedCpu (VflMultiFieldLine *multiFieldLine, const TopElement &seedElement, const AlgVector& seedPoint, int maxVertexCount);
  bool AdvectInterleavedGpu (VflMultiFieldLine *multiFieldLine, const TopElement &seedElement, const AlgVector& seedPoint, unsigned int queryObject);

  int Advect (
    float* arrayMagnitudes,
    float* arrayVertices,
    float* arrayTangents,
    int stride,
    const AlgVector&  seedPoint,
    const TopElement& seedElement,
    bool backward = false);

  bool  IntegrateEuler       (TopElement& curElement, AlgVector& curPosition, AlgVector& velocity, float timeStep);
  bool  IntegrateRungeKutta4 (TopElement& curElement, AlgVector& curPosition, AlgVector& velocity, float timeStep);
  bool  FindTarget           (TopElement& targetElement, const AlgVector& velocity, AlgVector &targetPosition);
  bool  IsElementActive      (const TopElement& topElement);
  
  bool CreateGpuAdvector (void);

private:

  TopModel *m_topModel;
  const bool *m_activeCells;
  TpvVectorFunction *m_vectorFunction;
  DsArray<AlgVector> m_seedPositions;
  DsArray<TopElement> m_seedElements;

  float        m_timeStep;
  unsigned int m_numStepsPerVertex;
  unsigned int m_maxVertexCount;

  int m_topModelElemCount;

  bool m_ownVectorFunction;
};

#endif // TPV_ADVECTION_H
