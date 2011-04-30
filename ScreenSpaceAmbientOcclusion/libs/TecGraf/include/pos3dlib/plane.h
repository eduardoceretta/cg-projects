/*************************************************************
*************************************************************/

#ifndef P3D_PLANE_H
#define P3D_PLANE_H

class VglViewSurface;

#include <alg/vector.h>
#include <alg/plane.h>
#include <alg/matrix.h>
#include <pos3dlib/defines.h>

class POS3DLIB_API P3DPlane
{
public:
  enum {
    POINT_CENTER = 0,
    POINT_BOTTOM_RIGHT,
    POINT_RIGHT,
    POINT_TOP_RIGHT,
    POINT_TOP,
    POINT_TOP_LEFT,
    POINT_LEFT,
    POINT_BOTTOM_LEFT,
    POINT_BOTTOM,
    POINT_MAX
  };

  /************************************************************************
   *                                                                     
   ************************************************************************/
  P3DPlane(VglViewSurface* surface=NULL);

  /************************************************************************
   *                                                                     
   ************************************************************************/
  ~P3DPlane();

  /************************************************************************
   *                                                                     
   ************************************************************************/
  AlgPlane GetPlane()
  {
    return AlgPlane(m_normal, -m_normal.Dot(m_center));
  }

  /************************************************************************
   *                                                                     
   ************************************************************************/
  void SetViewSurface(VglViewSurface* s)
  {
    m_surface = s;
  }

  /************************************************************************
   * Sets plane dimensions.                                                                    
   ************************************************************************/
  void SetSize(float width, float height);

  /************************************************************************
   *                                                                     
   ************************************************************************/
  float GetWidth()
  {
    return m_width;
  }

  /************************************************************************
   *                                                                     
   ************************************************************************/
  float GetHeight()
  {  
    return m_height;
  }

  /************************************************************************
   * Sets model bounding box used for plane fit.                                                                    
   ************************************************************************/
  void SetBox(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);
  
  /************************************************************************
   *                                                                     
   ************************************************************************/
  void Fit();

  /************************************************************************
   *  Sets plane center.                                                                    
   ************************************************************************/
  void SetCenter(AlgVector center);

  void SetCenter(float x, float y, float z)
  {
    SetCenter(AlgVector(x,y,z));
  }

  /************************************************************************
   *                                                                     
   ************************************************************************/
  AlgVector GetCenter()
  {
    return m_center;
  }

  void GetCenter(float* x, float* y, float* z)
  {
    *x = m_center.x; *y = m_center.y; *z = m_center.z;
  }

  /************************************************************************
   * Sets plane normal.                                                                    
   ************************************************************************/
  void SetNormal(AlgVector normal);

  void SetNormal(float x, float y, float z)
  {
    SetNormal(AlgVector(x,y,z));
  }

  /************************************************************************
   *                                                                     
   ************************************************************************/
  AlgVector GetNormal()
  {
    return m_normal;
  }

  void GetNormal(float* x, float* y, float* z)
  {
    *x = m_normal.x; *y = m_normal.y; *z = m_normal.z;
  }

  /************************************************************************
   *                                                                     
   ************************************************************************/
  void GetEquation(float* a, float* b, float* c, float* d)
  {
    *a = m_normal.x;
    *b = m_normal.y;
    *c = m_normal.z;
    *d = -(m_center.x*m_normal.x + m_center.y*m_normal.y + m_center.z*m_normal.z);
  }

  /*****************************************************************
   * Sets positions of the plane corners.
   * {BOTTOM_LEFT, BOTTOM_RIGHT, TOP_LEFT, TOP_RIGHT}
   *****************************************************************/
  void SetCorners(float* corners)
  {
    float bl[3], br[3], tl[3], tr[3];

    bl[0] = corners[0];
    bl[1] = corners[1];
    bl[2] = corners[2];

    br[0] = corners[3];
    br[1] = corners[4];
    br[2] = corners[5];

    tr[0] = corners[6];
    tr[1] = corners[7];
    tr[2] = corners[8];

    tl[0] = corners[9];
    tl[1] = corners[10];
    tl[2] = corners[11];

    SetPoint(POINT_BOTTOM_LEFT,   bl[0], bl[1], bl[2]);
    SetPoint(POINT_BOTTOM_RIGHT,  br[0], br[1], br[2]);
    SetPoint(POINT_TOP_LEFT,      tl[0], tl[1], tl[2]);
    SetPoint(POINT_TOP_RIGHT,     tr[0], tr[1], tr[2]);
  }

  /************************************************************************
  *  
  ************************************************************************/
  void GetCorners(float* corners)
  {
    AlgVector bl = GetPoint(POINT_BOTTOM_LEFT);
    AlgVector br = GetPoint(POINT_BOTTOM_RIGHT);
    AlgVector tl = GetPoint(POINT_TOP_LEFT);
    AlgVector tr = GetPoint(POINT_TOP_RIGHT);

    corners[0]  = bl.x;
    corners[1]  = bl.y;
    corners[2]  = bl.z;

    corners[3]  = br.x;
    corners[4]  = br.y;
    corners[5]  = br.z;

    corners[6]  = tr.x;
    corners[7]  = tr.y;
    corners[8]  = tr.z;

    corners[9]  = tl.x;
    corners[10] = tl.y;
    corners[11] = tl.z;
  }

  /************************************************************************
   *  Sets plane up vector.                                                                   
   ************************************************************************/
  void SetUp(AlgVector up);

  void SetUp(float x, float y, float z)
  {
    SetUp(AlgVector(x,y,z));
  }

  /************************************************************************
   *                                                                     
   ************************************************************************/
  AlgVector GetUp()
  {
    return m_up;
  }
  void GetUp(float* x, float* y, float* z)
  {
    *x = m_up.x; *y = m_up.y; *z = m_up.z;
  }

  /************************************************************************
   * Sets plane right vector.                                                                     
   ************************************************************************/
  void SetRight(AlgVector right);

  void SetRight(float x, float y, float z)
  {
    SetRight(AlgVector(x,y,z));
  }

  /************************************************************************
   *                                                                     
   ************************************************************************/
  AlgVector GetRight()
  {
    return m_right;
  }

  void GetRight(float* x, float* y, float* z)
  {
    *x = m_right.x; *y = m_right.y; *z = m_right.z;
  }

  /************************************************************************
   * Gets one of the plane points, defined by enum in this class.                                                                    
   ************************************************************************/
  void SetPoint(int i, AlgVector point);

  void SetPoint(int i, float x, float y, float z)
  {
    SetPoint(i, AlgVector(x,y,z));
  }

  AlgVector GetPoint(int i)
  {
    return m_points[i];
  }

  void GetPoint(int i, float* x, float* y, float* z)
  {
    *x = m_points[i].x; *y = m_points[i].y; *z = m_points[i].z;
  }

  void UpdatePoints();

private:
  VglViewSurface* m_surface;
  AlgVector m_center;
  AlgVector m_normal;
  AlgVector m_up;
  AlgVector m_right;
  AlgVector m_minbox;
  AlgVector m_maxbox;
  float m_width, m_height;
  AlgVector m_points[9];
  bool m_selection;
  bool m_ortho;
  float m_radius;
  float m_computed_radius;
  float m_r, m_g, m_b, m_a;
  float m_mask_r, m_mask_g, m_mask_b, m_mask_a;
  bool m_mask_enabled;
};



#endif
