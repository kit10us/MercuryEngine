// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <melua/Util.h>
#include <melua/component/GameComponent.h>
#include <melua/exports/ExportGame.h>
#include <melua/CreateState.h>

using namespace melua;
using namespace melua;
using namespace component;

namespace {
	std::map< std::string, int, unify::CaseInsensitiveLessThanTest > g_ValuesMap
	{
		{ "luaName", 0 },
		{ "path", 1 },
	};

	std::vector< std::string > g_ValuesList
	{
		{ "luaName" },
		{ "path" },
	};
}

char* GameComponent::Name()
{
	return "LuaGameScript";
}

GameComponent::GameComponent( lua_State * state, std::string luaName, unify::Path path )
	: me::game::GameComponent( Name() )
	, m_state( state )
	, m_luaName( luaName )
	, m_path( path )
{
}

GameComponent::~GameComponent()
{
	m_state = 0;
}

void GameComponent::CallMember( std::string function )
{
	// Get our _ENV...
	if ( !lua_getfield( m_state, LUA_REGISTRYINDEX, m_luaName.c_str() ) )						   
	{
		Error( m_state, "GameComponent not found! (" + m_luaName + ")" );
	}

	int r2 = lua_getfield( m_state, -1, function.c_str() );

	if ( r2 != 0 )
	{
		try
		{
			if ( lua_pcall( m_state, 0, 0, 0 ) != 0 )
			{
				std::string error = lua_tostring( m_state, -1 );
				Error( m_state, "Failed in script \"" + m_path.ToString() +"\" in function " + function + ": " + error );
			}
		}
		catch ( std::exception ex )
		{
			Error( m_state, "Exception in script \"" + m_path.ToString() +"\" in function " + function + ":\n " + ex.what() );
		}
	}
	else
	{
		lua_pop( m_state, 1 );
	}

	// Pop our _ENV.
	lua_pop( m_state, 1 );
}

void GameComponent::OnAttach( me::game::IGame * gameInstance )
{
	me::game::GameComponent::OnAttach( gameInstance );

	// Setup the script...	
	int result = luaL_loadfile( m_state, m_path.ToString().c_str() );
	if ( result == LUA_ERRSYNTAX )
	{
		Error( m_state, luaL_checkstring( m_state, -1 ) );
	}
	else if ( result == LUA_ERRFILE )
	{
		Error( m_state, "Failure trying to read script \"" + m_path.ToString() + "\"!" );
	}
	else if ( result != LUA_OK )
	{
		Error( m_state, "Failure in script!" );
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
		Error( m_state, "Failed with script initial call: " + error );
	}

	CallMember( "OnInit" );
}
		 
void GameComponent::OnBeforeStartup()
{
	CallMember( "OnBeforeStartup" );
}

void GameComponent::OnAfterStartup()
{
	CallMember( "OnAfterStartup" );
}

void GameComponent::OnUpdate( const me::UpdateParams & params )
{
	CallMember( "OnUpdate" );
}

void GameComponent::OnRender( me::render::Params params )
{
	CallMember( "OnRender" );
}

void GameComponent::OnDetach( me::game::IGame * gameInstance )
{
	me::game::GameComponent::OnDetach( gameInstance );
}

std::string GameComponent::GetWhat() const
{
	return std::string();
}

int GameComponent::GetValueCount() const
{
	return me::game::GameComponent::GetValueCount() + (int)g_ValuesList.size();
}

bool GameComponent::ValueExists( std::string name ) const
{
	auto && itr = g_ValuesMap.find( name );
	if( itr != g_ValuesMap.end() )
	{
		return true;
	}
	return me::game::GameComponent::ValueExists( name );
}

std::string GameComponent::GetValueName( int index ) const
{
	if( index < me::game::GameComponent::GetValueCount() )
	{
		return me::game::GameComponent::GetValueName( index );
	}

	int localIndex = index - me::game::GameComponent::GetValueCount();
	if( localIndex < (int)g_ValuesList.size() )
	{
		return g_ValuesList[localIndex];
	}

	return std::string();
}

int GameComponent::FindValueIndex( std::string name ) const
{
	auto && itr = g_ValuesMap.find( name );
	if( itr != g_ValuesMap.end() )
	{
		return itr->second;
	}

	return me::game::GameComponent::FindValueIndex( name );
}

bool GameComponent::SetValue( int index, std::string value )
{
	if( index < me::game::GameComponent::GetValueCount() )
	{
		return me::game::GameComponent::SetValue( index, value );
	}

	int localIndex = index - me::game::GameComponent::GetValueCount();
	switch( localIndex )
	{
	case 0:
		return false;

	case 1:
		return false;

	default:
		return false;
	}
}

std::string GameComponent::GetValue( int index ) const
{
	if( index < me::game::GameComponent::GetValueCount() )
	{
		return me::game::GameComponent::GetValue( index );
	}

	int localIndex = index - me::game::GameComponent::GetValueCount();
	switch( localIndex )
	{
	case 0:
		return m_luaName;

	case 1:
		return m_path.ToString();

	default:
		return std::string();
	}
}
