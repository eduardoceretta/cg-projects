/* *************************************************************
 *
 * Class:   resultfield.h
 * Created: 22/09/2006 - 12:09
 * Author:  rodesp
 *
 * Description: 
 *
 * *************************************************************/

#ifndef P3D_RESULT_FIELD_H
#define P3D_RESULT_FIELD_H

#include <ds/array.h>

class P3DModel;
class P3DResultCase;
class P3DResultStep;

#include <pos3dlib/defines.h>
#include "result.h"

/*****************************************************************
  class P3DResultField

    Result type and result field information are represented in
  this class.
*****************************************************************/
class POS3DLIB_API P3DResultField
{  
  P3DResultStep* m_step;
  int            m_i;         // Field index in field array of step.
  int            m_type_id;
  int            m_id;        // Field id.
  char           m_name[32];
  P3DResult*     m_result;
  float          m_vmin[P3D_RESULT_MAX_DIMENSION];
  float          m_vmax[P3D_RESULT_MAX_DIMENSION];
  bool           m_update_limits;


public:
  /** Basic result types. */
  enum ResultType {
    NONE = 0,
    NODAL,
    ELEMENT_NODAL,
    ELEMENT_GAUSS,
    ELEMENT,

    NODAL_VECTOR,
    ELEMENT_NODAL_VECTOR,
    ELEMENT_GAUSS_VECTOR,
    ELEMENT_VECTOR,

    ELEMENT_MATERIAL,
    NODAL_DISPLACEMENT,
    MAX_RESULT_TYPE
  };

  P3DResultField(int type_id, int id=-1, const char* name="");

  ~P3DResultField();

  /**
      Gets the type of this result field.
    @see ResultType
  */
  int GetTypeId()
  {
    return m_type_id;
  }

  void SetId(int id)
  {
    m_id = id;
  }

  /**
      Gets the id of this result field.
  */
  int GetId()
  {
    return m_id;
  }

  void SetName(const char* name)
  {
    strncpy(m_name, name, 30);
  }

  /**
    Gets the name of this result field.
  */
  char* GetName()
  {
    return m_name;
  }

  /**
    Gets dimension of this result type.
  */
  int GetDimension()
  {
    if (m_result)
      return m_result->GetDimension();
    else
      return 0;
  }

  void SetResult(P3DResult* result);

  /**
    Gets the result data of this field.
  */
  P3DResult* GetResult()
  {
    return m_result;
  }

  void  SetStep(P3DResultStep* res_step, int i);

  /**
    Gets the parent result step.
  */
  P3DResultStep* GetStep()
  {
    return m_step;
  }

  /**
    Gets global limits of the field data for all the steps.
  */
  void GetLimits(float* vmin, float* vmax);

  //TODO remove this method
  /** Gets the index of this field in the corresponding step. */
  int GetIndex ()
  {
    return m_i;
  }

private:
  void UpdateLimits();
};

#endif