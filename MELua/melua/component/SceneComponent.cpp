// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <melua/Util.h>
#include <melua/component/SceneComponent.h>
#include <melua/ExportGame.h>
#include <melua/CreateState.h>
#include <melua/ExportScene.h>

using namespace melua;
using namespace component;
using namespace me;

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

SceneComponent::SceneComponent( me::IGame * game, lua_State * state, std::string luaName, unify::Path path )
	: me::scene::SceneComponent( game->GetOS(), "LuaScene" )
	, m_game{ game }
	, m_state( state )
	, m_luaName( luaName )
	, m_path( path )
{
}

SceneComponent::~SceneComponent()
{
	m_state = 0;
}

void SceneComponent::CallMember( std::string function )
{
	// Get our _ENV...
	if ( !lua_getfield( m_state, LUA_REGISTRYINDEX, m_luaName.c_str() ) )						   
	{
		Error( m_state, "SceneComponent not found! (" + m_luaName + ")" );
	}

	int r2 = lua_getfield( m_state, -1, function.c_str() );

	if ( r2 != 0 )
	{
		try
		{
			if ( lua_pcall( m_state, 0, 0, 0 ) != 0 )
			{
				std::string error = lua_tostring( m_state, -1 );
				m_game->ReportError( me::ErrorLevel::Failure, "LUA", "Failed in script \"" + m_path.ToString() +"\" in function " + function + ": " + error );
			}
		}
		catch ( std::exception ex )
		{
			m_game->ReportError( me::ErrorLevel::Failure, "LUA", "Exception in script \"" + m_path.ToString() +"\" in function " + function + ":\n " + ex.what() );
		}
	}
	else
	{
		lua_pop( m_state, 1 );
	}

	// Pop our _ENV.
	lua_pop( m_state, 1 );
}

void SceneComponent::OnAttach( me::scene::IScene * scene )
{
	me::scene::SceneComponent::OnAttach( scene );

	int top = 0;
	std::string type;

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

	top = lua_gettop( m_state );
	assert( top == 1 );

	// Create table for modules _ENV table.
	lua_newtable( m_state ); // 2 - table

	// Add field name:scene->GetName()
	lua_pushstring( m_state, scene->GetName().c_str() ); // 2 - scene->GetName()
	lua_setfield( m_state, 2, "name" ); // 2 - table
	
	// Add variable this:scene
	{
		PushScene( m_state, scene ); // 3 - Scene
		lua_setfield( m_state, 2, "this" ); // 2 - table
	}

	top = lua_gettop( m_state );
	assert( top == 2 );

	// Create new metatable for __index to be _G (so missed functions, non-member functions, look in _G).
	lua_newtable( m_state );
	lua_getglobal( m_state, "_G" );
	lua_setfield( m_state, -2, "__index" );
	lua_setmetatable( m_state, -2 ); // Set global as the metatable

	top = lua_gettop( m_state );

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
	}

	CallMember( "OnInit" );
}

void SceneComponent::OnDetach( me::scene::IScene * scene )
{
}

void SceneComponent::OnBeforeStart()
{
	scene::SceneComponent::OnBeforeStart();
	CallMember( "OnBeforeStart" );
}

void SceneComponent::OnAfterStart()
{
	CallMember( "OnAfterStart" );
	scene::SceneComponent::OnAfterStart();
}

void SceneComponent::OnUpdate( UpdateParams params )
{
	scene::SceneComponent::OnUpdate( params );
	CallMember( "OnUpdate" );
}

void SceneComponent::OnSuspend()
{
	CallMember( "OnSuspend" );
}

void SceneComponent::OnResume()
{
	CallMember( "OnResume" );
}

void SceneComponent::OnEnd()
{
	CallMember("OnEnd");
	scene::SceneComponent::OnEnd();
}

std::string SceneComponent::GetWhat() const
{
	return std::string();
}
