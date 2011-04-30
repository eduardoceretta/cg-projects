/* *************************************************************
 *
 * Class:   resultstep.h
 * Created: 22/09/2006 - 12:09
 * Author:  rodesp
 *
 * Description: 
 *
 * *************************************************************/

#ifndef P3D_RESULT_STEP_H
#define P3D_RESULT_STEP_H

#include <ds/array.h>

class P3DModel;
class P3DResultCase;
#include <pos3dlib/defines.h>
#include "result.h"
#include "resultfield.h"

/**************************************************
  class P3DResultStep

    Represents a simulation time step.
**************************************************/
class POS3DLIB_API P3DResultStep
{
  P3DResultCase*  m_case;
  int             m_i;

  int             m_id;
  char            m_name[32];
  float           m_time;
  float           m_load_factor;
  float           m_def_deform_factor;
  bool            m_has_def_deform_factor;
  DsArray<P3DResultField*> m_fields[P3DResultField::MAX_RESULT_TYPE];

public:
  P3DResultStep(int id=-1, const char* name="") 
    : m_case(0), m_i(-1), m_id(id), m_time(0), m_load_factor(0),
    m_def_deform_factor(0.0f), m_has_def_deform_factor(false)
  { 
    strcpy(m_name, name); 
  }

  ~P3DResultStep()
  {
    for (int i = 0; i < P3DResultField::MAX_RESULT_TYPE; i++) 
      for (int j = 0; j < m_fields[i].Size(); j++)
        delete m_fields[i][j];
  }

  void SetId(int id) 
  { 
    m_id = id; 
  }

  /**
    Gets the user-defined id associated to this step.
    The id may be any valid integer.
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
      Gets the name of this step.
  */
  char* GetName()
  {
    return m_name;
  }

  void SetTime(float t)
  {
    m_time = t;
  }

  /**
      Gets the time associated to this step.
  */
  float GetTime()
  {
    return m_time;
  }

  void SetLoadFactor(float f)
  {
    m_load_factor = f;
  }

  /**
      Gets the load factor associated to this step.
  */
  float GetLoadFactor()
  {
    return m_load_factor;
  }

  void AddField(P3DResultField* field)
  {
    int type_id = field->GetTypeId();
    field->SetStep(this, m_fields[type_id].Size());
    m_fields[type_id].Append(field);
  }

  int  GetNumOfFields(int type_id)
  {
    return m_fields[type_id].Size();
  }

  /**
    Gets the i'th field (0-indexed) of the given type in this step.
    See P3DResultField::ResultType.
  */
  P3DResultField* GetField(int type_id, int i)
  {
    return m_fields[type_id].Get(i);
  }

  void  SetCase(P3DResultCase* res_case, int i)
  {
    m_case = res_case;
    m_i = i;
  }

  /**
      Gets the parent result case.
  */
  P3DResultCase* GetCase()
  {
    return m_case;
  }

  /**
      Gets the index of this step (0-indexed) in the list of steps of the
    parent result case.
  */
  int GetIndex()
  {
    return m_i;
  }

  void SetDefaultDeformFactor(float f)
  {
    m_def_deform_factor = f;
  }

  bool HasDefaultDeformFactor()
  {
    return m_has_def_deform_factor;
  }

  /**
      Gets the default deformation factor computed for this step.
  */
  float GetDefaultDeformFactor()
  {
    return m_def_deform_factor;
  }
};

#endif
