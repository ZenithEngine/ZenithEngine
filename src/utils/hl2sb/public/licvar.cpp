//===== Copyright � 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
//===========================================================================//

#define leiface_cpp

#include "Color.h"
#include "convar.h"
#include "../luamanager.h"
#include "lColor.h"
#include "lconvar.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


static int cvar_AllocateDLLIdentifier (lua_State *L) {
  lua_pushinteger(L, cvar->AllocateDLLIdentifier());
  return 1;
}

static int cvar_ConsoleColorPrintf (lua_State *L) {
  cvar->ConsoleColorPrintf(*(Color *)luaL_checkcolor(L, 1), luaL_checkstring(L, 2));
  return 0;
}

static int cvar_ConsoleDPrintf (lua_State *L) {
  cvar->ConsoleDPrintf(luaL_checkstring(L, 1));
  return 0;
}

static int cvar_ConsolePrintf (lua_State *L) {
  cvar->ConsolePrintf(luaL_checkstring(L, 1));
  return 0;
}

static int cvar_FindVar (lua_State *L) {
  lua_pushconvar(L, const_cast<ConVar *>(cvar->FindVar(luaL_checkstring(L, 1))));
  return 1;
}

static int cvar_GetCommandLineValue (lua_State *L) {
  lua_pushstring(L, cvar->GetCommandLineValue(luaL_checkstring(L, 1)));
  return 1;
}

static int cvar_RevertFlaggedConVars (lua_State *L) {
  cvar->RevertFlaggedConVars(luaL_checkint(L, 1));
  return 0;
}


static const luaL_Reg cvarlib[] = {
  {"AllocateDLLIdentifier",   cvar_AllocateDLLIdentifier},
  {"ConsoleColorPrintf",   cvar_ConsoleColorPrintf},
  {"ConsoleDPrintf",   cvar_ConsoleDPrintf},
  {"ConsolePrintf",   cvar_ConsolePrintf},
  {"FindVar",   cvar_FindVar},
  {"GetCommandLineValue",   cvar_GetCommandLineValue},
  {"RevertFlaggedConVars",   cvar_RevertFlaggedConVars},
  {NULL, NULL}
};


void CV_GlobalChange_Lua( IConVar *var, const char *pOldString, float flOldValue )
{
  lua_getfield(L, LUA_GLOBALSINDEX, "cvar");
  if (!lua_istable(L, -1)) {
    lua_pop(L, 1);
    return;
  }
  lua_getfield(L, -1, "CallGlobalChangeCallbacks");
  if (lua_isfunction(L, -1)) {
    lua_remove(L, -2);
	lua_pushconvar(L, (ConVar *)var);
    lua_pushstring(L, pOldString);
    lua_pushnumber(L, flOldValue);
    lua_call(L, 3, 0);
  }
  else
    lua_pop(L, 2);
}

void InstallGlobalChangeCallbacks( void )
{
	// Install the global cvar callback
	cvar->InstallGlobalChangeCallback( CV_GlobalChange_Lua );
}

void RemoveGlobalChangeCallbacks( void )
{
	// Remove the global cvar callback
	cvar->RemoveGlobalChangeCallback( CV_GlobalChange_Lua );
}


/*
** Open cvar library
*/
int luaopen_cvar (lua_State *L) {
  luaL_register(L, "cvar", cvarlib);
  lua_pop(L, 1);
  InstallGlobalChangeCallbacks();
  return 1;
}

