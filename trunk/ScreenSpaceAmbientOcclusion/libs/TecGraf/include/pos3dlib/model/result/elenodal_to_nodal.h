/**
*/

#ifndef P3D_ELENODAL_TO_NODAL_H
#define P3D_ELENODAL_TO_NODAL_H

#include <tops/tops.h>

#include <pos3dlib/defines.h>
#include "../result.h"

class POS3DLIB_API P3DEleNodalToNodal : public P3DResult
{
  TopModel  * m_top_model;                        //Modelo da tops
  P3DResult * m_result;                           //Resultado P3D
  float     * m_elenodal_data;                    //Resultado Elenodal
  int         m_size;                             //Tamanho do vetor de resultados Nodal
  float     * m_nodal_data;                       //Resultado Nodal
  int         m_ref_count;                        //Para garantir alocacao de dados no load 1 unica vez 
  float       m_vmin[P3D_RESULT_MAX_DIMENSION];   //Valor minimo por dimensao
  float       m_vmax[P3D_RESULT_MAX_DIMENSION];   //Valor maximo por dimensao


public:

  P3DEleNodalToNodal(TopModel* top_model, P3DResult* result);
  virtual ~P3DEleNodalToNodal();

  void Transform();

  //Metodos herdados
  virtual void GetLimits(float* vmin, float* vmax);
  virtual int GetSize();
  virtual float* GetData();
  virtual int Load();
  virtual int Unload();

  virtual void GetHistory(int id, float* data);
};

#endif