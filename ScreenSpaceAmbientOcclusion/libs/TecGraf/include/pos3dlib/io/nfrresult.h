/* *************************************************************
 *
 * Class:   nfrresult.h
 * Created: 22/09/2006 - 11:15
 * Author:  rodesp
 *
 * Description: 
 *
 * *************************************************************/

#ifndef P3D_NFR_RESULT_H
#define P3D_NFR_RESULT_H

#include <ds/hash.h>
#include <nfr/nfr.h>
#include <pos3dlib/defines.h>
#include "../model/model.h"

class P3DModel;
class P3DNfrModelReader;

class POS3DLIB_API P3DNfrResult : public P3DResult
{
  P3DModel* m_model;
  P3DNfrModelReader* m_reader;
  int    m_size;
  float* m_data;
  int    m_ref_count;
  int    m_nfr_type_id;
  int    m_nfr_field_id;
  int    m_dim;
  float  m_vmin[P3D_RESULT_MAX_DIMENSION];
  float  m_vmax[P3D_RESULT_MAX_DIMENSION];

public:
  P3DNfrResult(P3DModel* model, 
               P3DNfrModelReader* reader, 
               int nfr_type_id, 
               int nfr_field_id, 
               P3DResult::Location location, 
               int dim);
  virtual ~P3DNfrResult();

  int GetNfrResultType() { return m_nfr_type_id; }
  int GetNfrFieldId() { return m_nfr_field_id; }

  void GetLimits(float* vmin, float* vmax);
  int GetSize();
  float* GetData();
  int  GetDimension();
  int  Load();
  int  Unload();

  virtual void GetHistory(int id, float* data);
};

#endif