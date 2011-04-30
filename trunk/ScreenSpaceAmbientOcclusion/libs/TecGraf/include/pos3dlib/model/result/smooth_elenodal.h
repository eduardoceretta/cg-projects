/**
*/

#ifndef P3D_SMOOTH_ELENODAL_H
#define P3D_SMOOTH_ELENODAL_H

#include <tops/tops.h>
#include <pos3dlib/defines.h>
#include "../result.h"

class POS3DLIB_API P3DSmoothEleNodal : public P3DResult
{
public:
  P3DSmoothEleNodal(P3DModel* model, P3DResult* result);
  virtual ~P3DSmoothEleNodal();

  void Transform();

  virtual void GetLimits(float* vmin, float* vmax);
  virtual int GetSize();
  virtual float* GetData();
  virtual int GetDimension();
  virtual int Load();
  virtual int Unload();

  virtual void GetHistory(int id, float* data);

  virtual void Print();

private:
  P3DModel* m_model;              
  P3DResult* m_result;                           //Resultado P3D
  float* m_elenodal_data;                    //Resultado Elenodal
  int m_size;                             //Tamanho do vetor de resultados Nodal
  float* m_smooth_data;                       //Resultado Nodal
  int m_ref_count;                        //Para garantir alocacao de dados no load 1 unica vez 
  int m_dim;                              //Dimension
  float m_vmin[P3D_RESULT_MAX_DIMENSION];   //Valor minimo por dimensao
  float m_vmax[P3D_RESULT_MAX_DIMENSION];   //Valor maximo por dimensao
};

#endif