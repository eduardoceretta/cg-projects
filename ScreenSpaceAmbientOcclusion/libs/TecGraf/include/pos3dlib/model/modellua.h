/* *************************************************************
 *
 * Class:   modellua.h
 * Created: 25/5/2006 - 12:08
 * Author:  rodesp
 * $Id: modellua.h,v 1.2 2010/12/03 18:43:10 rodesp Exp $
 *
 * Description: 
 *
 * *************************************************************/

#ifndef P3D_MODEL_LUA_H
#define P3D_MODEL_LUA_H

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <pos3dlib/defines.h>
#include "model.h"

class POS3DLIB_API P3DModelLuaConsistencyListener : public P3DModelConsistencyListener
{
public:
  P3DModelLuaConsistencyListener();
  virtual ~P3DModelLuaConsistencyListener();

  /** 
      Sets lua function called by the method CheckManifoldFailed().
      function(node_id) ... return [true/false] end
  */
  void SetManifoldFailedFunc(lua_State* L, int lo);

  /** 
      Sets lua function called by the method CheckCohesiveFailed().
      function(node_id) ... return [true/false] end
  */
  void SetCohesiveFailedFunc(lua_State* L, int lo);

  /** Delegates event to the registered lua function. */
  virtual bool ManifoldFailed(TopModel* m, TopNode n);

  /** Delegates event to the registered lua function. */
  virtual bool CohesiveFailed(TopModel* m, TopElement coh);

private:
  struct LuaData
  {
    lua_State* L;
    int lo;
    LuaData(lua_State* L=NULL, int lo=-1) : L(L), lo(lo) {}
  };
  LuaData m_manifold_func;
  LuaData m_cohesive_func;
};


class POS3DLIB_API P3DModelLuaErrorListener : public P3DModelErrorListener
{
public:
  P3DModelLuaErrorListener();
  virtual ~P3DModelLuaErrorListener();

  /** 
      Sets lua function called by the method ErrorReported().
      function(error:<string>) ... end
  */
  void SetErrorReportedFunc(lua_State* L, int lo);

  /** Delegates event to the registered lua function. */
  virtual bool ErrorReported(const TopErrorHandler* e);

private:
  struct LuaData
  {
    lua_State* L;
    int lo;
    LuaData(lua_State* L=NULL, int lo=-1) : L(L), lo(lo) {}
  };
  LuaData m_error_func;
};

#endif