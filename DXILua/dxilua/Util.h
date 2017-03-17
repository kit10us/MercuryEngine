#pragma once

#include <dxilua/ScriptEngine.h>

void ShowStack( lua_State *L );
std::string GetTypename( lua_State *L, int index );