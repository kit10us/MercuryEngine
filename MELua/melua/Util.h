#pragma once

#include <melua/ScriptEngine.h>

void ShowStack( lua_State *L );

std::string GetTypename( lua_State *L );
std::string GetTypename( lua_State *L, int index );