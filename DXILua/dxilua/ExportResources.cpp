// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/ExportResources.h>
#include <dxilua/ScriptEngine.h>
#include <dxi/core/Game.h>

using namespace dxilua;
using namespace dxi;

extern "C"
int Resources_AddResource( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 3 );

	std::string type = lua_tostring( state, 1 );
	std::string name = lua_tostring( state, 2 );
	std::string path = lua_tostring( state, 3 );

	auto game = dynamic_cast< dxi::core::Game * >( ScriptEngine::GetGame() );

	try
	{
		game->GetResourceHub().Load( type, name, path );
	}
	catch( std::string ex )
	{
		game->ReportError( dxi::ErrorLevel::Failure, "Lua", ex );
	}
	catch( ... )
	{
		game->ReportError( dxi::ErrorLevel::Failure, "Lua", "Unknown error" );
	}

	return 0;
}

static const luaL_Reg resourcesFuncs[] =
{
	{ "add", Resources_AddResource },
	{ nullptr, nullptr }
};

int ExportResources( lua_State * state )
{
	luaL_newlib( state, resourcesFuncs );
	lua_setglobal( state, "resources" );
	return 1;
}
