/* *************************************************************
 *
 * Class:   modelreaderlua.h
 * Created: 25/5/2006 - 12:09
 * Author:  malf
 * $Id: modelreaderlua.h,v 1.2 2010/12/03 18:43:10 rodesp Exp $
 *
 * Description: 
 *
 * *************************************************************/

#ifndef P3D_MODEL_READER_LUA_H
#define P3D_MODEL_READER_LUA_H

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <pos3dlib/defines.h>
#include "modelreader.h"

/**
    Lua binding for P3DModelReaderListener.
*/
class POS3DLIB_API P3DModelReaderLuaListener : public P3DModelReaderListener
{
public:
  P3DModelReaderLuaListener();

  virtual ~P3DModelReaderLuaListener();

 /** 
      Sets lua function called by the method WarningReported().
      function(msg) end
  */
  void SetWarningReportedFunc(lua_State* L, int lo);

  /** 
      Sets lua function called by the method ProgressLabelChanged().
      function(msg) ... return [true/false] end
  */
  void SetProgressLabelChangedFunc(lua_State* L, int lo);

  /** 
      Sets lua function called by the method ProgressRatioChanged().
      function(ratio) ... return [true/false] end
  */
  void SetProgressRatioChangedFunc(lua_State* L, int lo);

  virtual void WarningReported(const char* msg);

  virtual bool ProgressLabelChanged(const char* msg);

  virtual bool ProgressRatioChanged(double ratio);

private:
  struct LuaData
  {
    lua_State* L;
    int lo;
    LuaData(lua_State* L=NULL, int lo=-1) : L(L), lo(lo) {}
  };
  LuaData m_warning_func;
  LuaData m_progress_label_func;
  LuaData m_progress_ratio_func;
};

#endif