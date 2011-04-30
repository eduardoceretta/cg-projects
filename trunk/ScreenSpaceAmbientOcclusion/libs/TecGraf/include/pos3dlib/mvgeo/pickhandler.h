#ifndef P3D_RENDER_ENGINE_PICK_HANDLER_H
#define P3D_RENDER_ENGINE_PICK_HANDLER_H

#include <tops/tops.h>
#include <vgl/vgl.h>
#include <pos3dlib/defines.h>
#include "renderengine.h"
#include "../handler/pickhandler.h"


class POS3DLIB_API P3DRenderEnginePickHandlerListener
{
public:
  virtual ~P3DRenderEnginePickHandlerListener () {}
  virtual void NodePicked (int id, float sx, float sy, float wx, float wy, float wz) = 0;
  virtual void ElementPicked (int id, float sx, float sy, float wx, float wy, float wz) = 0;
};


/**
  Class for picking element/node/plane.
  Usage: 
  1 - Create the handler for the given VglCanvas (or VglSubCanvas) and P3DRenderEngine.
  2 - Set the desired pick mode (currently, PICK_ELEMENT or PICK_NODE - iherited
  from P3DPickHandler class).
    Note: SetSelectionRenderFunc() is used internally by the handler and
    should not be set.
  3 - Set a pick listener (see P3DRenderEnginePickHandlerListener).
  4 - Adds the handler to the canvas (or subcanvas) and activate it like any other VglHandler.
  When the user clicks over a node or element, respectively, the handler will
  invoke the pick listener.
*/
class POS3DLIB_API P3DRenderEnginePickHandler : public P3DPickHandler
{
public:
  static const char* CLASSNAME () { return "P3DRenderEnginePickHandler"; }
  virtual const char* ClassName () const { return CLASSNAME(); }

  P3DRenderEnginePickHandler (VglCanvas* canvas, P3DRenderEngine* engine);  
  P3DRenderEnginePickHandler (VglSubCanvas* subcanvas, P3DRenderEngine* engine);  
  virtual ~P3DRenderEnginePickHandler ();

  void SetListener (P3DRenderEnginePickHandlerListener* listener);

  P3DRenderEnginePickHandlerListener* GetListener () const
  {
    return m_listener;
  }

  /**
    Sets the current pick type (PICK_ELEMENT or PICK_NODE).
    Default: PICK_NODE.
  */
  void SetPickType (unsigned int type);

  unsigned int GetPickType () const 
  {
    return m_pick_type;
  }

  P3DRenderEngine* GetRenderEngine () const
  {
    return m_render_engine;
  }

protected:
  virtual void ProcessFunction (VglSelectionHit* hits, int n);

private:       
  static void PickFunc (P3DPickHandler* h, void* data);
  static void RenderFunc (void* data);

  VglCanvas* m_canvas;
  VglSubCanvas* m_subcanvas;
  P3DRenderEngine* m_render_engine;
  unsigned int m_pick_type;
  P3DRenderEnginePickHandlerListener* m_listener;
};

#endif

