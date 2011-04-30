/**
*/

#ifndef P3D_SCALAR_TO_VECTOR_H
#define P3D_SCALAR_TO_VECTOR__H

#include <tops/tops.h>

#include <pos3dlib/defines.h>
#include "../result.h"

//////////////////////////////////////////////////////////////////////////

class POS3DLIB_API P3DScalarToVector : public P3DResult
{
  P3DResult* m_resultx;
  P3DResult* m_resulty;
  P3DResult* m_resultz;
  float*     m_data; 
  int        m_size;
  int        m_coord;
  int        m_ref_count; //Para garantir alocacao de dados no load 1 unica vez 
  float      m_vmin[3];
  float      m_vmax[3];

public:

  P3DScalarToVector(P3DResult* rx, P3DResult* ry, P3DResult* rz);
  virtual ~P3DScalarToVector();

  //Metodos herdados
  virtual void GetLimits(float* vmin, float* vmax);
  virtual int GetSize();
  virtual float* GetData();
  virtual int GetDimension();
  virtual int Load();
  virtual int Unload();

  virtual void GetHistory(int id, float* data);

  virtual void Print();
  
};

#endif