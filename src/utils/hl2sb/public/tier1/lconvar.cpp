//===== Copyright � 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
// $Workfile:     $
// $Date:         $
//
//-----------------------------------------------------------------------------
// $NoKeywords: $
//===========================================================================//

#define lconvar_cpp

#include "convar.h"
#include "tier1/utldict.h"
#include "../../luamanager.h"
#include "lconvar.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

/*
** access functions (stack -> C)
*/


LUA_API lua_ConCommand *lua_toconcommand (lua_State *L, int idx) {
  lua_ConCommand **ppConCommand = (lua_ConCommand **)luaL_checkudata(L, idx, "ConCommand");
  return *ppConCommand;
}


LUA_API lua_ConVar *lua_toconvar (lua_State *L, int idx) {
  lua_ConVar **ppConVar = (lua_ConVar **)luaL_checkudata(L, idx, "ConVar");
  return *ppConVar;
}



/*
** push functions (C -> stack)
*/


LUA_API void lua_pushconcommand (lua_State *L, lua_ConCommand *pConCommand) {
  if (pConCommand == NULL)
    lua_pushnil(L);
  else {
    lua_ConCommand **ppConCommand = (lua_ConCommand **)lua_newuserdata(L, sizeof(lua_ConCommand));
    *ppConCommand = pConCommand;
    luaL_getmetatable(L, "ConCommand");
    lua_setmetatable(L, -2);
  }
}


LUA_API void lua_pushconvar (lua_State *L, lua_ConVar *pConVar) {
  if (pConVar == NULL)
    lua_pushnil(L);
  else {
    lua_ConVar **ppConVar = (lua_ConVar **)lua_newuserdata(L, sizeof(lua_ConVar));
    *ppConVar = pConVar;
    luaL_getmetatable(L, "ConVar");
    lua_setmetatable(L, -2);
  }
}


LUALIB_API lua_ConCommand *luaL_checkconcommand (lua_State *L, int narg) {
  lua_ConCommand *d = lua_toconcommand(L, narg);
  if (d == NULL)  /* avoid extra test when d is not 0 */
    luaL_typerror(L, narg, "ConCommand");
  return d;
}


LUALIB_API lua_ConVar *luaL_checkconvar (lua_State *L, int narg) {
  lua_ConVar *d = lua_toconvar(L, narg);
  if (d == NULL)  /* avoid extra test when d is not 0 */
    luaL_typerror(L, narg, "ConVar");
  return d;
}


static int ConCommand_CanAutoComplete (lua_State *L) {
  lua_pushboolean(L, luaL_checkconcommand(L, 1)->CanAutoComplete());
  return 0;
}

static int ConCommand_IsCommand (lua_State *L) {
  lua_pushboolean(L, luaL_checkconcommand(L, 1)->IsCommand());
  return 0;
}

static int ConCommand___tostring (lua_State *L) {
  lua_pushfstring(L, "ConCommand: \"%s\"", luaL_checkconcommand(L, 1)->GetName());
  return 1;
}


static const luaL_Reg ConCommandmeta[] = {
  {"CanAutoComplete", ConCommand_CanAutoComplete},
  {"IsCommand", ConCommand_IsCommand},
  {"__tostring", ConCommand___tostring},
  {NULL, NULL}
};


static CUtlDict< ConCommand*, unsigned short > m_ConCommandDatabase;

void CC_ConCommand( const CCommand& args )
{
#if 0
#ifdef CLIENT_DLL
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
#else
	CBasePlayer* pPlayer = UTIL_GetCommandClient();
#endif
	const char *pCmd = args[0];

	// Is the client spawned yet?
	if ( !pPlayer )
		return;

	MDLCACHE_CRITICAL_SECTION();

	lua_getglobal( L, "concommand" );
	if ( lua_istable( L, -1 ) )
	{
		lua_getfield( L, -1, "Dispatch" );
		if ( lua_isfunction( L, -1 ) )
		{
			lua_remove( L, -2 );
			lua_pushplayer( L, pPlayer );
			lua_pushstring( L, pCmd );
			lua_pushstring( L, args.ArgS() );
			lua_call( L, 3, 1 );
			if ( lua_isboolean( L, -1 ) )
			{
				bool res = (bool)luaL_checkboolean( L, -1 );
				lua_pop( L, 1 );
				if ( !res )
				{
#ifndef CLIENT_DLL
					if ( Q_strlen( pCmd ) > 128 )
					{
						ClientPrint( pPlayer, HUD_PRINTCONSOLE, "Console command too long.\n" );
					}
					else
					{
						// tell the user they entered an unknown command
						ClientPrint( pPlayer, HUD_PRINTCONSOLE, UTIL_VarArgs( "Unknown command: %s\n", pCmd ) );
					}
#endif
				}
			}
			else
			{
				lua_pop( L, 1 );
			}
		}
		else
			lua_pop( L, 2 );
	}
	else
		lua_pop( L, 1 );
#endif
}

static int luasrc_ConCommand (lua_State *L) {
  const char *pName = luaL_checkstring(L, 1);
  // Complain about duplicately defined ConCommand names...
  unsigned short lookup = m_ConCommandDatabase.Find( pName );
  if ( lookup != m_ConCommandDatabase.InvalidIndex() || cvar->FindCommand(pName) )
  {
    lua_pushconcommand(L, cvar->FindCommand(pName));
    return 1;
  }

  //ConCommand *pConCommand = new ConCommand(strdup(pName), CC_ConCommand, strdup(luaL_optstring(L, 2, 0)), luaL_optint(L, 3, 0), NULL);

  //lookup = m_ConCommandDatabase.Insert( pName, pConCommand );
  //Assert( lookup != m_ConCommandDatabase.InvalidIndex() );
  //lua_pushconcommand(L, pConCommand);
  return 1;
}

void ResetConCommandDatabase( void )
{
	int c = m_ConCommandDatabase.Count(); 
	for ( int i = 0; i < c; ++i )
	{
		ConCommand *pConCommand = m_ConCommandDatabase[ i ];
		cvar->UnregisterConCommand(pConCommand);
		delete pConCommand;
	}
	m_ConCommandDatabase.RemoveAll();
}


static const luaL_Reg ConCommand_funcs[] = {
  {"ConCommand", luasrc_ConCommand},
  {NULL, NULL}
};


/*
** Open ConCommand object
*/
int luaopen_ConCommand (lua_State *L) {
  luaL_newmetatable(L, "ConCommand");
  luaL_register(L, NULL, ConCommandmeta);
  lua_pushvalue(L, -1);  /* push metatable */
  lua_setfield(L, -2, "__index");  /* metatable.__index = metatable */
  lua_pushstring(L, "concommand");
  lua_setfield(L, -2, "__type");  /* metatable.__type = "concommand" */
  luaL_register(L, "_G", ConCommand_funcs);
  lua_pop(L, 2);
  return 1;
}


static int ConVar_AddFlags (lua_State *L) {
  luaL_checkconvar(L, 1)->AddFlags(luaL_checkinteger(L, 2));
  return 0;
}

static int ConVar_GetBool (lua_State *L) {
  lua_pushboolean(L, luaL_checkconvar(L, 1)->GetBool());
  return 1;
}

static int ConVar_GetDefault (lua_State *L) {
  lua_pushstring(L, luaL_checkconvar(L, 1)->GetDefault());
  return 1;
}

static int ConVar_GetFloat (lua_State *L) {
  lua_pushnumber(L, luaL_checkconvar(L, 1)->GetFloat());
  return 1;
}

static int ConVar_GetHelpText (lua_State *L) {
  lua_pushstring(L, luaL_checkconvar(L, 1)->GetHelpText());
  return 1;
}

static int ConVar_GetInt (lua_State *L) {
  lua_pushinteger(L, luaL_checkconvar(L, 1)->GetInt());
  return 1;
}

static int ConVar_GetMax (lua_State *L) {
  float maxVal;
  lua_pushboolean(L, luaL_checkconvar(L, 1)->GetMax(maxVal));
  lua_pushnumber(L, maxVal);
  return 2;
}

static int ConVar_GetMin (lua_State *L) {
  float minVal;
  lua_pushboolean(L, luaL_checkconvar(L, 1)->GetMin(minVal));
  lua_pushnumber(L, minVal);
  return 2;
}

static int ConVar_GetName (lua_State *L) {
  lua_pushstring(L, luaL_checkconvar(L, 1)->GetName());
  return 1;
}

static int ConVar_GetString (lua_State *L) {
  lua_pushstring(L, luaL_checkconvar(L, 1)->GetString());
  return 1;
}

static int ConVar_IsCommand (lua_State *L) {
  lua_pushboolean(L, luaL_checkconvar(L, 1)->IsCommand());
  return 1;
}

static int ConVar_IsFlagSet (lua_State *L) {
  lua_pushboolean(L, luaL_checkconvar(L, 1)->IsFlagSet(luaL_checkinteger(L, 2)));
  return 1;
}

static int ConVar_IsRegistered (lua_State *L) {
  lua_pushboolean(L, luaL_checkconvar(L, 1)->IsRegistered());
  return 1;
}

static int ConVar_Revert (lua_State *L) {
  luaL_checkconvar(L, 1)->Revert();
  return 0;
}

static int ConVar_SetValue (lua_State *L) {
  switch(lua_type(L, 2)) {
	case LUA_TNUMBER:
	  luaL_checkconvar(L, 1)->SetValue((float)luaL_checknumber(L, 2));
	  break;
	case LUA_TSTRING:
	default:
	  luaL_checkconvar(L, 1)->SetValue(luaL_checkstring(L, 2));
	  break;
  }
  return 0;
}

static int ConVar___tostring (lua_State *L) {
  lua_pushfstring(L, "ConVar: \"%s\" = \"%s\"", luaL_checkconvar(L, 1)->GetName(), luaL_checkconvar(L, 1)->GetString());
  return 1;
}


static const luaL_Reg ConVarmeta[] = {
  {"AddFlags", ConVar_AddFlags},
  {"GetBool", ConVar_GetBool},
  {"GetDefault", ConVar_GetDefault},
  {"GetFloat", ConVar_GetFloat},
  {"GetHelpText", ConVar_GetHelpText},
  {"GetInt", ConVar_GetInt},
  {"GetMax", ConVar_GetMax},
  {"GetMin", ConVar_GetMin},
  {"GetName", ConVar_GetName},
  {"GetString", ConVar_GetString},
  {"IsCommand", ConVar_IsCommand},
  {"IsFlagSet", ConVar_IsFlagSet},
  {"IsRegistered", ConVar_IsRegistered},
  {"Revert", ConVar_Revert},
  {"SetValue", ConVar_SetValue},
  {"__tostring", ConVar___tostring},
  {NULL, NULL}
};


static CUtlDict< ConVar*, unsigned short > m_ConVarDatabase;

static int luasrc_ConVar (lua_State *L) {
  const char *pName = luaL_checkstring(L, 1);
  // Complain about duplicately defined ConVar names...
  unsigned short lookup = m_ConVarDatabase.Find( pName );
  if ( lookup != m_ConVarDatabase.InvalidIndex() || cvar->FindVar(pName) )
  {
    lua_pushconvar(L, cvar->FindVar(pName));
    return 1;
  }

  //ConVar *pConVar = new ConVar(strdup(pName), luaL_checkstring(L, 2), luaL_optint(L, 3, 0), strdup(luaL_optstring(L, 4, 0)), luaL_optboolean(L, 5, 0), luaL_optnumber(L, 6, 0.0), luaL_optboolean(L, 7, 0), luaL_optnumber(L, 8, 0));

  //lookup = m_ConVarDatabase.Insert( pName, pConVar );
  //Assert( lookup != m_ConVarDatabase.InvalidIndex() );
  //lua_pushconvar(L, pConVar);
  return 1;
}

void ResetConVarDatabase( void )
{
	int c = m_ConVarDatabase.Count(); 
	for ( int i = 0; i < c; ++i )
	{
		ConVar *pConVar = m_ConVarDatabase[ i ];
		cvar->UnregisterConCommand(pConVar);
		delete pConVar;
	}
	m_ConVarDatabase.RemoveAll();
}


static const luaL_Reg ConVar_funcs[] = {
  {"ConVar", luasrc_ConVar},
  {NULL, NULL}
};


/*
** Open ConVar object
*/
int luaopen_ConVar (lua_State *L) {
  luaL_newmetatable(L, "ConVar");
  luaL_register(L, NULL, ConVarmeta);
  lua_pushvalue(L, -1);  /* push metatable */
  lua_setfield(L, -2, "__index");  /* metatable.__index = metatable */
  lua_pushstring(L, "convar");
  lua_setfield(L, -2, "__type");  /* metatable.__type = "convar" */
  luaL_register(L, "_G", ConVar_funcs);
  lua_pop(L, 2);
  return 1;
}

