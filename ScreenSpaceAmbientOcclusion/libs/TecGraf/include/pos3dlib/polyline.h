
#ifndef P3D_POLYLINE_H
#define P3D_POLYLINE_H

#include <ds/array.h>
#include <alg/vector.h>
#include <pos3dlib/defines.h>

class POS3DLIB_API P3DPolyline
{
public:
  P3DPolyline ()
  {
  }

  P3DPolyline (const P3DPolyline& o)
  {
    m_points.Append(o.m_points.Size(), o.m_points.GetArrayConst()); 
  }

  ~P3DPolyline ()
  {
  }

  P3DPolyline* Copy()
  {
    return new P3DPolyline(*this);
  }

  int GetNumPoints () const 
  { 
    return m_points.Size(); 
  }

  void Clear () 
  { 
    m_points.Empty(); 
  }

  bool IsEmpty () const
  {
    return m_points.Size() == 0;
  }

  void Set (int i, AlgVector p) 
  { 
    m_points.Set(i, p);
  }

  AlgVector Get (int i) const
  { 
    return m_points.Get(i);
  }

  void Append (AlgVector p) 
  { 
    m_points.Append(p);
  }

  void Append (float x, float y, float z)
  {
    m_points.Append(AlgVector(x, y, z));
  }

  void Remove(int i)
  {
    m_points.Remove(i);
  }

  void Invert ()
  {
    m_points.Invert();
  }

private:
  DsArray<AlgVector> m_points;
};

#endif