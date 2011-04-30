/**
* Modulo de definicao da classe TpvGeometry, para ser usado como geometria extraida para a TopsView
*/

#ifndef TPV_POINT_SET_2_H
#define TPV_POINT_SET_2_H

#include <tops/tops.h>
#include <topsview/defines.h>

#include "array.h"
#include "support.h"

#include <stdio.h>
#include <math.h>

/**  
*/
class TPV_API TpvPointSet2
{
private:
  int m_size;
  TpvFloatArray   m_pos;
  TpvSupportArray m_supports;
  TpvFloatArray   m_field;

public:  
  TpvPointSet2() : m_size(0) {}
  ~TpvPointSet2() {}

  void SetSize(int size) { m_size = size; }
  int GetSize() { return m_size; }

  TpvFloatArray* GetPositions() { return &m_pos; }

  /** Preferred support order: NODE, ELEMENT. */
  TpvSupportArray* GetSupports() { return &m_supports; }

  TpvFloatArray* GetField() { return &m_field; }

  void Clear()
  {
    m_size = 0;
    m_pos.Empty();
    m_supports.Empty();
    m_field.Empty();
  }

  void Print()
  {
    for (int i = 0; i < m_size; i++) {
      printf("point[%d]:\n", i);
      if (m_pos.Size() > 0) {
        printf("  pos: %+6.3e %+6.3e %+6.3e\n",  
          m_pos.Get(3*i), m_pos.Get(3*i+1), m_pos.Get(3*i+2));
      }
    }
  }
};


#endif
