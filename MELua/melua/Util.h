#pragma once

#include <melua/ScriptEngine.h>

std::string GetTypename( lua_State *L );
std::string GetTypename( lua_State *L, int index );