#ifndef TPV_POLYGON_SET_H
#define TPV_POLYGON_SET_H

#include "array.h"
#include "support.h"
#include <topsview/defines.h>

#include <stdio.h>
#include <math.h>

/**
    Represents polygonal data.
*/
class TPV_API TpvPolygonSet
{
private:
  int m_size;
  TpvIntArray     m_npoints;
  TpvFloatArray   m_pos;
  TpvFloatArray   m_normals;
  TpvSupportArray m_supports;
  TpvFloatArray   m_field;
  
public:
  TpvPolygonSet() : m_size(0) {}
  virtual ~TpvPolygonSet() {}

  void SetSize(int size) { m_size = size; }

  /** Gets the number of polygons. */
  int GetSize() { return m_size; }

  /** Gets the array with the number of points for each polygon. */
  TpvIntArray* GetNumOfPoints() { return &m_npoints; }

  /** Gets the array of positions of the points of the polygons. */
  TpvFloatArray* GetPositions() { return &m_pos; }

  /** Gets the array of positions of the points of the polygons. */
  TpvFloatArray* GetNormals() { return &m_normals; }

  /** Preferred support order: FACET_USE, ELEMENT. */
  TpvSupportArray* GetSupports() { return &m_supports; }

  /**
    Gets the array of the property field associated to each point of
    the polygons.
  */
  TpvFloatArray* GetField() { return &m_field; }

  void Clear() 
  {
    m_size = 0;
    m_npoints.Empty();
    m_pos.Empty();
    m_normals.Empty();
    m_supports.Empty();
    m_field.Empty();
  }

  void Print() 
  {
    float* ppos = m_pos.GetArray();
    float* pfield = m_field.GetArray();
    for (int i = 0; i < m_size; i++) {
      printf("polygon[%d]:\n", i);
      for (int pi = 0; pi < m_npoints.Get(i); pi++) {
        if (m_pos.Size() > 0) {
          printf("  pos[%d]: %+6.3e %+6.3e %+6.3e\n", pi, ppos[0], ppos[1], ppos[2]);
          ppos += 3;
        }
        if (m_field.Size() > 0) {
          printf("  field[%d]: %+6.3e\n", pi, pfield[0]);
          pfield++;
        }
      }
    }
  }
};

#endif
