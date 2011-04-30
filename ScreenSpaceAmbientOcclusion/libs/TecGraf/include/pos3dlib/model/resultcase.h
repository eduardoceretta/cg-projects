/* *************************************************************
 *
 * Class:   resultcase.h
 * Created: 22/09/2006 - 12:09
 * Author:  rodesp
 * $Id: resultcase.h,v 1.3 2010/12/03 18:43:10 rodesp Exp $
 *
 * Description: 
 *
 * *************************************************************/

#ifndef P3D_RESULT_CASE_H
#define P3D_RESULT_CASE_H

#include <ds/array.h>

class P3DModel;
#include <pos3dlib/defines.h>
#include "result.h"
#include "resultfield.h"
#include "resultstep.h"

/*****************************************************************
  class P3DResultCase

    Represents a simulation case.
*****************************************************************/
class POS3DLIB_API P3DResultCase
{
  P3DModel*           m_model;
  int                 m_id;
  char                m_name[32];
  DsArray<P3DResultStep*> m_steps;

public:
  P3DResultCase(int id=-1, const char* name="") 
    : m_model(0), m_id(id)
  { 
    strcpy(m_name, name); 
  }

  ~P3DResultCase()
  {
    for (int i = 0; i < m_steps.Size(); i++)
      delete m_steps[i];
  }

  void SetModel(P3DModel* model)
  {
    m_model = model;
  }

  /**
      Gets the model which this result case is part of.
  */
  P3DModel* GetModel()
  {
    return m_model;
  }

  void SetId(int id) 
  { 
    m_id = id; 
  }

  /**
      Gets the id of this result case.
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
      Gets the name of this result case.
  */
  char* GetName()
  {
    return m_name;
  }

  void AddStep(P3DResultStep* step)
  {
    step->SetCase(this, m_steps.Size());
    m_steps.Append(step);
  }

  int  GetNumOfSteps()
  {
    return m_steps.Size();
  }

  /**
      Gets the ith step of this case (0-indexed).
  */
  P3DResultStep* GetStep(int i)
  {
    return m_steps.Get(i);
  }
};

#endif
