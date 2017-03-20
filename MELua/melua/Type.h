// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <lua.hpp>
#include <string.h>
#include <list>

namespace melua
{
	struct Type
	{
		struct NameFunctionPair
		{
			std::string name;
			lua_CFunction function;
		};


		Type();

		Type(std::string name, const luaL_Reg * functions, int functionCount, lua_CFunction constructor = 0, lua_CFunction collector = 0);
		
		std::string name;
		const luaL_Reg * functions;
		int functionCount;
		lua_CFunction constructor;
		lua_CFunction collector;
		lua_CFunction newindex;
		lua_CFunction index;
		lua_CFunction add;
		lua_CFunction sub;
		lua_CFunction mul;
		lua_CFunction div;
		lua_CFunction mod;
		lua_CFunction pow;
		lua_CFunction unm;
		lua_CFunction concat;
		lua_CFunction len;
		lua_CFunction eq;
		lua_CFunction lt;
		lua_CFunction le;
		std::list< NameFunctionPair > named_constructors;
	};
}