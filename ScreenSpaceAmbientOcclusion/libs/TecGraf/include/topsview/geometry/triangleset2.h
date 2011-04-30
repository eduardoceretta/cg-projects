#ifndef TPV_TRIANGLE_SET2_H
#define TPV_TRIANGLE_SET2_H

#include "array.h"
#include "support.h"
#include <topsview/defines.h>

#include <stdio.h>
#include <math.h>

/**
    Represents triangle data.
*/
class TPV_API TpvTriangleSet2
{
private:
  int m_size;
  TpvFloatArray m_pos;
  TpvFloatArray m_normals;
  TpvSupportArray m_supports;
  TpvFloatArray m_params;
  TpvFloatArray m_field;
  TpvFloatArray m_field2;

public:
  TpvTriangleSet2() : m_size(0) {}
  ~TpvTriangleSet2() {}

  void SetSize(int size) { m_size = size; }
  int GetSize() { return m_size; }

  TpvFloatArray* GetPositions() { return &m_pos; }

  TpvFloatArray* GetNormals() { return &m_normals; }

  /** Preferred support order: FACET_USE/ELEMENT2D, ELEMENT. */
  TpvSupportArray* GetSupports() { return &m_supports; }

  // optional
  TpvFloatArray* GetParams() { return &m_params; }

  // optional
  TpvFloatArray* GetField() { return &m_field; }

  // optional
  TpvFloatArray* GetField2() { return &m_field2; }

  void Clear() 
  {
    m_size = 0;
    m_pos.Empty();
    m_normals.Empty();
    m_supports.Empty();
    m_params.Empty();
    m_field.Empty();
    m_field2.Empty();
  }

  void Print()
  {
    for (int i = 0; i < m_size; i++) {
      printf("triangle[%d]:\n", i);
      for (int j = 0; j < 3; j++) {
        if (m_pos.Size() > 0) {
          printf("  pos[%d]: %+6.3e %+6.3e %+6.3e\n", j, 
            m_pos.Get(9*i+3*j), m_pos.Get(9*i+3*j+1), m_pos.Get(9*i+3*j+2));
        }
        if (m_normals.Size() > 0) {
          printf("  normal[%d]: %+6.3e %+6.3e %+6.3e\n", j, 
            m_normals.Get(9*i+3*j), m_normals.Get(9*i+3*j+1), m_normals.Get(9*i+3*j+2));
        }
      }
    }
    printf("param[]:\n");
    for (int i = 0; i < m_params.Size(); i++)
      printf("  param[%d]: %+6.3e\n", i, m_params.Get(i));
    printf("field[]:\n");
    for (int i = 0; i < m_field.Size(); i++)
      printf("  field[%d]: %+6.3e\n", i, m_field.Get(i));
    printf("field2[]:\n");
    for (int i = 0; i < m_field2.Size(); i++)
      printf("  field2[%d]: %+6.3e\n", i, m_field2.Get(i));
  }
};

#endif
