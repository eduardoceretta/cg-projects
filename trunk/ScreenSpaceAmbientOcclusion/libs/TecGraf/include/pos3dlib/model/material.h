/************************************************************************
  P3DMaterial class
  Represents a finite element material.
  rodesp@tecgraf.puc-rio.br
*************************************************************************/

#ifndef P3D_MATERIAL_H
#define P3D_MATERIAL_H

#include <pos3dlib/defines.h>

/********************************************
  class P3DMaterial
*********************************************/
class POS3DLIB_API P3DMaterial
{
  int m_id;
  char m_name[32];

public:
  P3DMaterial(int id=-1, const char* name="")
    : m_id(id)
  {
    strncpy(m_name, name, 30); 
  }

  ~P3DMaterial() {}

  void SetId(int id)
  {
    m_id = id;
  }

  /**
      Gets the id of this material.
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
      Gets the name of this material.
  */
  char* GetName()
  {
    return m_name;
  }
};

#endif