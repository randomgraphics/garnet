/*
** $Id: lapi.h,v 1.3 2005/01/04 03:10:10 t-cheli Exp $
** Auxiliary functions from Lua API
** See Copyright Notice in lua.h
*/

#ifndef lapi_h
#define lapi_h


#include "lobject.h"


void luaA_pushobject (lua_State *L, const TValue *o);

#endif
