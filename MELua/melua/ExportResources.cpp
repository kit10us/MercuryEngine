// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <melua/ExportResources.h>
#include <melua/ScriptEngine.h>
#include <me/Game.h>

using namespace melua;
using namespace me;

extern "C"
int Resources_AddResource( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 3 );

	std::string type = lua_tostring( state, 1 );
	std::string name = lua_tostring( state, 2 );
	std::string path = lua_tostring( state, 3 );

	auto game = dynamic_cast< Game * >( ScriptEngine::GetGame() );

	try
	{
		game->GetResourceHub().Load( type, name, path );
	}
	catch( std::string ex )
	{
		game->ReportError( me::ErrorLevel::Failure, "Lua", ex );
	}
	catch( ... )
	{
		game->ReportError( me::ErrorLevel::Failure, "Lua", "Unknown error" );
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
