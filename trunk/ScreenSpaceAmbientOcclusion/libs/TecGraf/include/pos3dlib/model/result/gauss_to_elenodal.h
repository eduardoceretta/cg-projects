/**
*/

#ifndef P3D_GAUSS_TO_ELENODAL_H
#define P3D_GAUSS_TO_ELENODAL_H

#include <tops/tops.h>

#include <pos3dlib/defines.h>
#include "../model.h"

/*************************************************************************
  class P3DGaussToEleNodal

  Maps results from gauss points to element nodes, using the same
  extrapolation method as the old Pos3D.
**************************************************************************/
class POS3DLIB_API P3DGaussToEleNodal : public P3DResult
{
public:
  enum Method {
    EXTRAPOLATE_TO_NODES = 0,
    COPY_TO_NODES
  };

  P3DGaussToEleNodal(P3DModel* model, P3DResult* result);
  virtual ~P3DGaussToEleNodal();

  void Transform();

  virtual void GetLimits(float* vmin, float* vmax);
  virtual int GetSize();
  virtual float* GetData();
  virtual int GetDimension();
  virtual int Load();
  virtual int Unload();
  virtual void GetHistory(int id, float* data);

  void SetExtrapolationMethod(Method method)
  {
    m_method = method;
  }


private:
  int TransformElem(int elem_id, float* elegauss_data, float* elenodal_data);

private:
  P3DModel* m_model;
  TopModel* m_top_model;
  P3DResult* m_result;                           //Resultado P3D
  float* m_gauss_data;                       //Resultado de Gauss
  int m_size;                             //Tamanho do vetor de resultados Elenodal
  float* m_elenodal_data;                    //Resultado Elenodal
  int m_ref_count;                        //Para garantir alocacao de dados no load 1 unica vez 
  int m_dimension;
  float m_vmin[P3D_RESULT_MAX_DIMENSION];   //Valor minimo por dimensao
  float m_vmax[P3D_RESULT_MAX_DIMENSION];   //Valor maximo por dimensao
  Method m_method;
};

#endif