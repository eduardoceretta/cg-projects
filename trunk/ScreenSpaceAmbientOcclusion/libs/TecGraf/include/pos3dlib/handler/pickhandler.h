#ifndef P3D_PICK_HANDLER_H
#define P3D_PICK_HANDLER_H

#include <stdio.h>
#include <ds/array.h>
#include <alg/vector.h>
#include <vgl/hnd/pointpick.h>
#include <vgl/viewsurface.h>
#include <pos3dlib/defines.h>

//TODO Set size of select buffer (SetSelectionBufferSize()).

/**
  Class for picking element/node/plane.
  User must call inherited method SetSelectionRenderFunc()
  to set the function used to draw scene in selection mode.
*/
class POS3DLIB_API P3DPickHandler : public VglPointPickHandler
{
public:
  enum {
    PICK_NONE = 0,
    PICK_ELEMENT = 1,
    PICK_NODE = 2,
    PICK_PLANE = 4,
    PICK_ELEMENT_AND_NODE = 8,
    PICK_VIRTUAL_NODE = 16
  };

  typedef void (*PickCb) (P3DPickHandler* handler, void* data);

  static const char* CLASSNAME () { return "P3DPickHandler"; }
  virtual const char* ClassName () const { return CLASSNAME(); }

  P3DPickHandler(VglCanvas* canvas);  
  P3DPickHandler(VglSubCanvas* subcanvas);  
  virtual ~P3DPickHandler();

  /**
    Sets the function called when an item is picked.
  */
  void SetPickFunc(PickCb cb, void* data);

  /**
      Returns the picked object type (ELEMENT, NODE or PLANE).
  */
  unsigned int GetPickedType()
  {
    return m_picked_type;
  }

  /**
      Returns the id of the picked object.
  */
  unsigned int GetPickedId()
  {
    return m_picked_id;
  }

  /**
      Returns the position of the picked object in world coordinates.
  */
  void GetPickedWorldPos(float* wx, float* wy, float* wz)
  {
    *wx = m_picked_wx; 
    *wy = m_picked_wy; 
    *wz = m_picked_wz;
  }

  /**
      Returns the position of the picked object in screen coordinates.
  */
  void GetPickedScreenPos(float* x, float* y)
  {
    *x = m_picked_x; 
    *y = m_picked_y; 
  }

  /**
    Sets iup cursor.
    If name == nil, default cursor will be used.
  */
  void SetCursor(const char* name);

  virtual void Begin();
  virtual void End();
  virtual int MouseEntry(bool st);
  virtual int MouseMotion(int bt, float x, float y);
  virtual int MouseButton(int bt, int st, float x, float y);   
  virtual int PostRedraw (void);

protected:
  virtual void ProcessFunction (VglSelectionHit* hits, int n);
  void SetMousePos(float x, float y)
  {
    m_x = x;
    m_y = y;
  }
  void GetMousePos(float* x, float* y)
  {
    *x = m_x;
    *y = m_y;
  }
  void SetPickedType(int type)
  {
    m_picked_type = type;
  }
  void SetPickedId(unsigned int id)
  {
    m_picked_id = id;
  }
  void SetPickedScreenPos(float x, float y)
  {
    m_picked_x = x;
    m_picked_y = y;
  }
  void SetPickedWorldPos(float wx, float wy, float wz)
  {
    m_picked_wx = wx; 
    m_picked_wy = wy; 
    m_picked_wz = wz;
  }
  void InvokePickFunc()
  {
    if (m_pickcb)
      m_pickcb(this, m_pickcb_data);
  }
  void LoadCursor();
  void UnLoadCursor();
  void ApplyLoadedCursor();

private:       
  VglCanvas* m_canvas;
  VglSubCanvas* m_subcanvas;
  unsigned int m_picked_type;
  unsigned int m_picked_id;
  float m_x0, m_y0;
  float m_x, m_y;
  float m_picked_x, m_picked_y;
  float m_picked_wx, m_picked_wy, m_picked_wz;
  char m_cursor[64];
  char m_old_cursor[64];
  PickCb m_pickcb;
  void* m_pickcb_data; 
};

#endif

