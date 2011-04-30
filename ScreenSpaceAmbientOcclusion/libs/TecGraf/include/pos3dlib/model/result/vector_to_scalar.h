/**
*/

#ifndef P3D_VECTOR_TO_SCALAR_H
#define P3D_VECTOR_TO_SCALAR_H

#include <tops/tops.h>
#include <pos3dlib/defines.h>
#include "../result.h"

//////////////////////////////////////////////////////////////////////////

class POS3DLIB_API P3DVectorToScalar : public P3DResult
{
  P3DResult* m_result;
  float*     m_data; 
  int        m_size;
  int        m_coord;
  int        m_ref_count; //Para garantir alocacao de dados no load 1 unica vez 
  float      m_vmin;
  float      m_vmax;

public:

  P3DVectorToScalar(P3DResult* result, int coord);
  virtual ~P3DVectorToScalar();

  //Metodos herdados
  virtual void GetLimits(float* vmin, float* vmax);
  virtual int GetSize();
  virtual float* GetData();
  virtual int GetDimension();
  virtual int Load();
  virtual int Unload();

  virtual void GetHistory(int id, float* data);
};

#endif