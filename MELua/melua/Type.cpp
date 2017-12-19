// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <melua/Type.h>

using namespace melua;

Type::Type()
	: functions{ 0 }
	, functionCount{ 0 }
	, constructor{ 0 }
	, collector{ 0 }
	, newindex{ 0 }
	, add{ 0 }
	, sub{ 0 }
	, mul{ 0 }
	, div{ 0 }
	, mod{ 0 }
	, pow{ 0 }
	, unm{ 0 }
	, concat{ 0 }
	, len{ 0 }
	, eq{ 0 }
	, lt{ 0 }
	, le{ 0 }
{
}

Type::Type( std::string name, const luaL_Reg * functions, lua_CFunction constructor, lua_CFunction collector )
	: name{ name }
	, functions{ functions }
	, functionCount{ 0 }
	, constructor{ constructor }
	, collector{ collector }
	, newindex{ 0 }
	, index{ 0 }
	, add{ 0 }
	, sub{ 0 }
	, mul{ 0 }
	, div{ 0 }
	, mod{ 0 }
	, pow{ 0 }
	, unm{ 0 }
	, concat{ 0 }
	, len{ 0 }
	, eq{ 0 }
	, lt{ 0 }
	, le{ 0 }
{
	// Count functions.
	for( const auto * function = functions; function && function->name; function++ ) functionCount++;
}
