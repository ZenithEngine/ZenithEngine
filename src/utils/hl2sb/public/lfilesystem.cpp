//===== Copyright � 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
// $NoKeywords: $
//===========================================================================//

#define lfilesystem_cpp

#include "../luamanager.h"
#include "filesystem.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// Interfaces from the engine
extern IFileSystem *filesystem;



static int filesystem_AddPackFile (lua_State *L) {
  lua_pushboolean(L, filesystem->AddPackFile(luaL_checkstring(L, 1), luaL_checkstring(L, 2)));
  return 1;
}

static int filesystem_AddSearchPath (lua_State *L) {
  filesystem->AddSearchPath(luaL_checkstring(L, 1), luaL_checkstring(L, 2), (SearchPathAdd_t)luaL_optint(L, 3, PATH_ADD_TO_TAIL));
  return 0;
}

static int filesystem_BeginMapAccess (lua_State *L) {
  filesystem->BeginMapAccess();
  return 0;
}

static int filesystem_CancelWaitForResources (lua_State *L) {
  filesystem->CancelWaitForResources(luaL_checkint(L, 1));
  return 0;
}

static int filesystem_CreateDirHierarchy (lua_State *L) {
  filesystem->CreateDirHierarchy(luaL_checkstring(L, 1), luaL_optstring(L, 2, 0));
  return 0;
}

static int filesystem_DiscardPreloadData (lua_State *L) {
  filesystem->DiscardPreloadData();
  return 0;
}

static int filesystem_Disconnect (lua_State *L) {
  filesystem->Disconnect();
  return 0;
}

static int filesystem_EnableWhitelistFileTracking (lua_State *L) {
	//filesystem->EnableWhitelistFileTracking(luaL_checkboolean(L, 1));
  return 0;
}

static int filesystem_EndMapAccess (lua_State *L) {
  filesystem->EndMapAccess();
  return 0;
}

static int filesystem_FileExists (lua_State *L) {
  lua_pushboolean(L, filesystem->FileExists(luaL_checkstring(L, 1), luaL_optstring(L, 2, 0)));
  return 1;
}

static int filesystem_GetDVDMode (lua_State *L) {
  lua_pushinteger(L, filesystem->GetDVDMode());
  return 1;
}

static int filesystem_GetLocalCopy (lua_State *L) {
  filesystem->GetLocalCopy(luaL_checkstring(L, 1));
  return 0;
}

static int filesystem_GetWhitelistSpewFlags (lua_State *L) {
  lua_pushinteger(L, filesystem->GetWhitelistSpewFlags());
  return 1;
}

static int filesystem_HintResourceNeed (lua_State *L) {
  lua_pushinteger(L, filesystem->HintResourceNeed(luaL_checkstring(L, 1), luaL_checkint(L, 2)));
  return 1;
}

static int filesystem_Init (lua_State *L) {
  lua_pushinteger(L, filesystem->Init());
  return 1;
}

static int filesystem_IsDirectory (lua_State *L) {
  lua_pushboolean(L, filesystem->IsDirectory(luaL_checkstring(L, 1), luaL_optstring(L, 2, 0)));
  return 1;
}

static int filesystem_IsFileImmediatelyAvailable (lua_State *L) {
  lua_pushboolean(L, filesystem->IsFileImmediatelyAvailable(luaL_checkstring(L, 1)));
  return 1;
}

static int filesystem_IsFileWritable (lua_State *L) {
  lua_pushboolean(L, filesystem->IsFileWritable(luaL_checkstring(L, 1), luaL_optstring(L, 2, 0)));
  return 1;
}

static int filesystem_IsSteam (lua_State *L) {
  lua_pushboolean(L, filesystem->IsSteam());
  return 1;
}

static int filesystem_LoadCompiledKeyValues (lua_State *L) {
  filesystem->LoadCompiledKeyValues((IFileSystem::KeyValuesPreloadType_t)luaL_checkint(L, 1), luaL_checkstring(L, 2));
  return 0;
}

static int filesystem_MarkAllCRCsUnverified (lua_State *L) {
  filesystem->MarkAllCRCsUnverified();
  return 0;
}

static int filesystem_MarkPathIDByRequestOnly (lua_State *L) {
  filesystem->MarkPathIDByRequestOnly(luaL_checkstring(L, 1), luaL_checkboolean(L, 2));
  return 0;
}

static int filesystem_MountSteamContent (lua_State *L) {
  lua_pushinteger(L, filesystem->MountSteamContent(luaL_optint(L, 1, -1)));
  return 1;
}

static int filesystem_Precache (lua_State *L) {
  lua_pushboolean(L, filesystem->Precache(luaL_checkstring(L, 1), luaL_optstring(L, 2, 0)));
  return 1;
}

static int filesystem_PrintOpenedFiles (lua_State *L) {
  filesystem->PrintOpenedFiles();
  return 0;
}

static int filesystem_PrintSearchPaths (lua_State *L) {
  filesystem->PrintSearchPaths();
  return 0;
}

static int filesystem_RemoveAllSearchPaths (lua_State *L) {
  filesystem->RemoveAllSearchPaths();
  return 0;
}

static int filesystem_RemoveFile (lua_State *L) {
  filesystem->RemoveFile(luaL_checkstring(L, 1), luaL_optstring(L, 2, 0));
  return 0;
}

static int filesystem_RemoveSearchPath (lua_State *L) {
  lua_pushboolean(L, filesystem->RemoveSearchPath(luaL_checkstring(L, 1), luaL_optstring(L, 2, 0)));
  return 1;
}

static int filesystem_RemoveSearchPaths (lua_State *L) {
  filesystem->RemoveSearchPaths(luaL_checkstring(L, 1));
  return 0;
}

static int filesystem_RenameFile (lua_State *L) {
  lua_pushboolean(L, filesystem->RenameFile(luaL_checkstring(L, 1), luaL_checkstring(L, 2), luaL_optstring(L, 3, 0)));
  return 1;
}

static int filesystem_SetFileWritable (lua_State *L) {
  lua_pushboolean(L, filesystem->SetFileWritable(luaL_checkstring(L, 1), luaL_checkboolean(L, 2), luaL_optstring(L, 3, 0)));
  return 1;
}

static int filesystem_SetupPreloadData (lua_State *L) {
  filesystem->SetupPreloadData();
  return 0;
}

static int filesystem_SetWarningLevel (lua_State *L) {
  filesystem->SetWarningLevel((FileWarningLevel_t)luaL_checkint(L, 1));
  return 0;
}

static int filesystem_SetWhitelistSpewFlags (lua_State *L) {
  filesystem->SetWhitelistSpewFlags(luaL_checkint(L, 1));
  return 0;
}

static int filesystem_Shutdown (lua_State *L) {
  filesystem->Shutdown();
  return 0;
}

static int filesystem_Size (lua_State *L) {
  lua_pushinteger(L, filesystem->Size(luaL_checkstring(L, 1), luaL_optstring(L, 2, 0)));
  return 1;
}

static int filesystem_UnzipFile (lua_State *L) {
  lua_pushboolean(L, filesystem->UnzipFile(luaL_checkstring(L, 1), luaL_checkstring(L, 2), luaL_checkstring(L, 3)));
  return 1;
}

static int filesystem_WaitForResources (lua_State *L) {
  lua_pushinteger(L, filesystem->WaitForResources(luaL_checkstring(L, 1)));
  return 1;
}


static const luaL_Reg filesystemlib[] = {
  {"AddPackFile",   filesystem_AddPackFile},
  {"AddSearchPath",   filesystem_AddSearchPath},
  {"BeginMapAccess",   filesystem_BeginMapAccess},
  {"CancelWaitForResources",   filesystem_CancelWaitForResources},
  {"CreateDirHierarchy",   filesystem_CreateDirHierarchy},
  {"DiscardPreloadData",   filesystem_DiscardPreloadData},
  {"Disconnect",   filesystem_Disconnect},
  {"EnableWhitelistFileTracking",   filesystem_EnableWhitelistFileTracking},
  {"EndMapAccess",   filesystem_EndMapAccess},
  {"FileExists",   filesystem_FileExists},
  {"GetDVDMode",   filesystem_GetDVDMode},
  {"GetLocalCopy",   filesystem_GetLocalCopy},
  {"GetWhitelistSpewFlags",   filesystem_GetWhitelistSpewFlags},
  {"HintResourceNeed",   filesystem_HintResourceNeed},
  {"Init",   filesystem_Init},
  {"IsDirectory",   filesystem_IsDirectory},
  {"IsFileImmediatelyAvailable",   filesystem_IsFileImmediatelyAvailable},
  {"IsFileWritable",   filesystem_IsFileWritable},
  {"IsSteam",   filesystem_IsSteam},
  {"LoadCompiledKeyValues",   filesystem_LoadCompiledKeyValues},
  {"MarkAllCRCsUnverified",   filesystem_MarkAllCRCsUnverified},
  {"MarkPathIDByRequestOnly",   filesystem_MarkPathIDByRequestOnly},
  {"MountSteamContent",   filesystem_MountSteamContent},
  {"Precache",   filesystem_Precache},
  {"PrintOpenedFiles",   filesystem_PrintOpenedFiles},
  {"PrintSearchPaths",   filesystem_PrintSearchPaths},
  {"RemoveAllSearchPaths",   filesystem_RemoveAllSearchPaths},
  {"RemoveFile",   filesystem_RemoveFile},
  {"RemoveSearchPath",   filesystem_RemoveSearchPath},
  {"RemoveSearchPaths",   filesystem_RemoveSearchPaths},
  {"RenameFile",   filesystem_RenameFile},
  {"SetFileWritable",   filesystem_SetFileWritable},
  {"SetupPreloadData",   filesystem_SetupPreloadData},
  {"SetWarningLevel",   filesystem_SetWarningLevel},
  {"SetWhitelistSpewFlags",   filesystem_SetWhitelistSpewFlags},
  {"Shutdown",   filesystem_Shutdown},
  {"Size",   filesystem_Size},
  {"UnzipFile",   filesystem_UnzipFile},
  {"WaitForResources",   filesystem_WaitForResources},
  {NULL, NULL}
};


/*
** Open filesystem library
*/
int luaopen_filesystem (lua_State *L) {
  luaL_register(L, "filesystem", filesystemlib);
  lua_pop(L, 1);
  return 1;
}

