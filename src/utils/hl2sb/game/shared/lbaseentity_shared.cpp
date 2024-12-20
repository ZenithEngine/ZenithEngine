//===== Copyright � 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: The base class from which all game entities are derived.
//
//===========================================================================//

#define lbaseentity_shared_cpp

#include "baseentity_shared.h"
#include "baseentity.h"
#include "mapentities.h"
#include "mathlib/vector.h"
#include "tier1/strtools.h"
#include "../../luamanager.h"
#include "lbaseentity_shared.h"
#include "mathlib/lvector.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


/*
** access functions (stack -> C)
*/


LUA_API lua_CBaseEntity *lua_toentity (lua_State *L, int idx) {
  if (!lua_isuserdata(L, idx))
    luaL_typerror(L, idx, "CBaseEntity");
  if (lua_getmetatable(L, idx)) {
    lua_pushstring(L, "__type");
	lua_rawget(L, -2);
	if (!lua_isstring(L, -1))
	  lua_pop(L, 2);
	else if (Q_strcmp(luaL_checkstring(L, -1), "entity") != 0)
	  luaL_typerror(L, idx, "CBaseEntity");
  }
  lua_CBaseEntity **ppEntity = (lua_CBaseEntity **)lua_touserdata(L, idx);
  return *ppEntity;
}



/*
** push functions (C -> stack)
*/


LUA_API void lua_pushentity (lua_State *L, CBaseEntity *pEntity) {
  if (pEntity == NULL)
    lua_pushnil(L);
  else {
    lua_CBaseEntity **ppEntity = (lua_CBaseEntity **)lua_newuserdata(L, sizeof(pEntity));
    *ppEntity = pEntity;
    luaL_getmetatable(L, "CBaseEntity");
    lua_setmetatable(L, -2);
  }
}


LUALIB_API lua_CBaseEntity *luaL_checkentity (lua_State *L, int narg) {
  lua_CBaseEntity *d = lua_toentity(L, narg);
  if (d == NULL)  /* avoid extra test when d is not 0 */
    luaL_typerror(L, narg, "CBaseEntity");
  return d;
}


LUALIB_API lua_CBaseEntity *luaL_optentity (lua_State *L, int narg,
                                                          CBaseEntity *def) {
  return luaL_opt(L, luaL_checkentity, narg, def);
}


static int luasrc_CBaseEntity_GetClassname (lua_State *L) {
  lua_pushstring(L, CBaseEntity_GetClassname(luaL_checkentity(L, 1)));
  return 1;
}

static int luasrc_CBaseEntity_IsAlive (lua_State *L) {
  lua_pushboolean(L, CBaseEntity_IsAlive(luaL_checkentity(L, 1)));
  return 1;
}

static int luasrc_CBaseEntity_PrecacheModel (lua_State *L) {
  lua_pushinteger(L, CBaseEntity_PrecacheModel(luaL_checkstring(L, 1)));
  return 1;
}

static int luasrc_CBaseEntity_SetAbsOrigin (lua_State *L) {
  CBaseEntity_SetAbsOrigin(luaL_checkentity(L, 1), *(Vector *)luaL_checkvector(L, 2));
  return 0;
}

static int luasrc_CBaseEntity_SetModel (lua_State *L) {
  CBaseEntity_SetModel(luaL_checkentity(L, 1), luaL_checkstring(L, 2));
  return 0;
}

static int luasrc_CBaseEntity___tostring (lua_State *L) {
  lua_pushfstring(L, "CBaseEntity: %s", CBaseEntity_GetClassname(luaL_checkentity(L, 1)));
  return 1;
}


static const luaL_Reg CBaseEntitymeta[] = {
  {"GetClassname", luasrc_CBaseEntity_GetClassname},
  {"IsAlive", luasrc_CBaseEntity_IsAlive},
  {"PrecacheModel", luasrc_CBaseEntity_PrecacheModel},
  {"SetAbsOrigin", luasrc_CBaseEntity_SetAbsOrigin},
  {"SetModel", luasrc_CBaseEntity_SetModel},
  {"__tostring", luasrc_CBaseEntity___tostring},
  {NULL, NULL}
};


static int luasrc_CreateEntityByName (lua_State *L) {
  lua_pushentity(L, CreateEntityByName(luaL_checkstring(L, 1)));
  return 1;
}

static int luasrc_DispatchSpawn (lua_State *L) {
  lua_pushinteger(L, DispatchSpawn(luaL_checkentity(L, 1)));
  return 1;
}


static const luaL_Reg CBaseEntity_funcs[] = {
  {"CreateEntityByName", luasrc_CreateEntityByName},
  {"DispatchSpawn", luasrc_DispatchSpawn},
  {NULL, NULL}
};


/*
** Open CBaseEntity object
*/
int luaopen_CBaseEntity (lua_State *L) {
  luaL_newmetatable(L, "CBaseEntity");
  luaL_register(L, NULL, CBaseEntitymeta);
  lua_pushvalue(L, -1);  /* push metatable */
  lua_setfield(L, -2, "__index");  /* metatable.__index = metatable */
  lua_pushstring(L, "entity");
  lua_setfield(L, -2, "__type");  /* metatable.__type = "entity" */
  luaL_register(L, "_G", CBaseEntity_funcs);
  lua_pop(L, 2);
  return 1;
}

