
#ifndef P3D_VIRTUAL_NODE_H
#define P3D_VIRTUAL_NODE_H

#include <tops/model.h>
#include <pos3dlib/defines.h>

class POS3DLIB_API P3DVirtualNode 
{
public:
  /** 
    Creates a new virtual node, given a support element and 
    the natural coordinates of the node within the element.
  */
  P3DVirtualNode()
    : m_elem(TopElement()), m_x(0), m_y(0), m_z(0)
  {
    m_natcoords[0] = 0;
    m_natcoords[1] = 0;
    m_natcoords[2] = 0;
  }
  void SetElem(TopElement elem)
  {
    m_elem = elem;
  }
  /** Gets the support element of the node. */
  TopElement GetElem() const 
  { 
    return m_elem; 
  }
  void SetNatCoords(const float* natcoords)
  { 
    m_natcoords[0] = natcoords[0]; 
    m_natcoords[1] = natcoords[1]; 
    m_natcoords[2] = natcoords[2]; 
  }
  void SetNatCoords(float r, float s, float t)
  {
    m_natcoords[0] = r; 
    m_natcoords[1] = s; 
    m_natcoords[2] = t; 
  }
  /** Gets the natural coordinates of the node within the support element. */
  const float* GetNatCoords() const 
  { 
    return m_natcoords; 
  }
  void GetNatCoords(float* r, float* s, float* t)
  {
    *r = m_natcoords[0];
    *s = m_natcoords[1];
    *t = m_natcoords[2];
  }
  void SetPosition(float x, float y, float z)
  {
    m_x = x; 
    m_y = y; 
    m_z = z;
  }
  float GetX() const
  {
    return m_x;
  }
  float GetY() const
  {
    return m_y;
  }
  float GetZ() const
  {
    return m_z;
  }

private:
  TopElement m_elem;
  float m_x, m_y, m_z;
  float m_natcoords[3];
};

#endif
