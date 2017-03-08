// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/Util.h>
#include <dxilua/component/GameComponent.h>
#include <dxilua/ExportGame.h>
#include <dxilua/CreateState.h>

using namespace dxilua;
using namespace melua;
using namespace component;

namespace {
	std::map< std::string, int, unify::CaseInsensitiveLessThanTest > g_ValuesMap
	{
		{ "path", 0 },
	};

	std::vector< std::string > g_ValuesList
	{
		{ "path" },
	};
}

GameComponent::GameComponent( lua_State * state, me::IGame * game, std::string name, unify::Path path )
	: m_state( state )
	, m_game( game )
	, m_luaName( name )
	, m_path( path )
	, m_enabled( true )
{
}

GameComponent::~GameComponent()
{
	m_state = 0;
}

std::string GameComponent::GetName() const
{
	return "LUA Game Script";
}

void GameComponent::CallMember( std::string function )
{
	// Get our _ENV...
	if ( !lua_getfield( m_state, LUA_REGISTRYINDEX, m_luaName.c_str() ) )						   
	{
		m_game->ReportError( me::ErrorLevel::Failure, "LUA", "GameComponent not found! (" + m_luaName + ")" );
	}

	int r2 = lua_getfield( m_state, -1, function.c_str() );

	if ( r2 != 0 )
	{
		if ( lua_pcall( m_state, 0, 0, 0 ) != 0 )
		{
			std::string error = lua_tostring( m_state, -1 );
			m_game->ReportError( me::ErrorLevel::Failure, "LUA", "Failed in script " + function + ": " + error );
		}
	}
	else
	{
		lua_pop( m_state, 1 );
	}

	// Pop our _ENV.
	lua_pop( m_state, 1 );
}

void GameComponent::OnAttach( me::IGame * game )
{
	// Setup the script...	
	int result = luaL_loadfile( m_state, m_path.ToString().c_str() );
	if ( result == LUA_ERRSYNTAX )
	{
		m_game->ReportError( me::ErrorLevel::Failure, "Lua", luaL_checkstring( m_state, -1 ) );
		assert( 0 );
	}
	else if ( result == LUA_ERRFILE )
	{
		m_game->ReportError( me::ErrorLevel::Failure, "Lua", "Failure trying to read script \"" + m_path.ToString() + "\"!" );
	}
	else if ( result != LUA_OK )
	{
		m_game->ReportError( me::ErrorLevel::Failure, "Lua", "Failure in script!" );
	}

	// Create table for modules _ENV table.
	lua_newtable( m_state );

	// Add member variables.
	lua_pushstring( m_state, m_luaName.c_str() );
	lua_setfield( m_state, -2, "_name" );

	// Create new metatable for __index to be _G (so missed functions, non-member functions, look in _G).
	lua_newtable( m_state );
	lua_getglobal( m_state, "_G" );
	lua_setfield( m_state, -2, "__index" );
	lua_setmetatable( m_state, -2 ); // Set global as the metatable

	// Push to registery with a unique name.
	lua_setfield( m_state, LUA_REGISTRYINDEX, m_luaName.c_str() );

	// Retrieve registry.
	int i = lua_getfield( m_state, LUA_REGISTRYINDEX, m_luaName.c_str() );

	// Set the upvalue (_ENV)
	const char * uv = lua_setupvalue( m_state, -2, 1 );

	result = lua_pcall( m_state, 0, LUA_MULTRET, 0 );
	if ( result != LUA_OK )
	{
		std::string error = lua_tostring( m_state, -1 );
		m_game->ReportError( me::ErrorLevel::Failure, "LUA", "Failed with script initial call: " + error );
		assert( 0 ); // TODO:
	}

	CallMember( "OnInit" );
}

void GameComponent::OnDetach( me::IGame * game )
{
}
			 
void GameComponent::OnBeforeStartup( me::IGame * game )
{
	CallMember( "OnBeforeStartup" );
}

void GameComponent::OnAfterStartup( me::IGame * game )
{
	CallMember( "OnAfterStartup" );
}

void GameComponent::OnUpdate( me::IGame * game, me::UpdateParams params )
{
	CallMember( "OnUpdate" );
}

void GameComponent::OnRender( me::IGame * game, me::RenderParams params )
{
	CallMember( "OnRender" );
}


