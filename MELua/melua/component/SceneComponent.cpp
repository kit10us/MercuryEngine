// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <melua/Util.h>
#include <melua/component/SceneComponent.h>
#include <melua/exports/ExportGame.h>
#include <melua/CreateState.h>
#include <melua/exports/ExportScene.h>

using namespace melua;
using namespace component;
using namespace me;

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

SceneComponent::SceneComponent( me::game::IGame * gameInstance, lua_State * state, std::string luaName, unify::Path path )
	: me::scene::SceneComponent( gameInstance->GetOS(), "LuaScene" )
	, m_game{ gameInstance }
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

	auto * se = ScriptEngine::GetInstance();

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

	lua_getglobal( m_state, (scene->GetName() + "_env").c_str()  );
	lua_setupvalue( m_state, -2, 1 );

	se->AssertTop( 1 );

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

	se->AssertTop( 2 );

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

void SceneComponent::OnUpdate( const UpdateParams & params ) 
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

int SceneComponent::GetValueCount() const
{
	return me::scene::SceneComponent::GetValueCount() + (int)g_ValuesList.size();
}

bool SceneComponent::ValueExists( std::string name ) const
{
	auto && itr = g_ValuesMap.find( name );
	if( itr != g_ValuesMap.end() )
	{
		return true;
	}
	return me::scene::SceneComponent::ValueExists( name );
}

std::string SceneComponent::GetValueName( int index ) const
{
	if( index < me::scene::SceneComponent::GetValueCount() )
	{
		return me::scene::SceneComponent::GetValueName( index );
	}

	int localIndex = index - me::scene::SceneComponent::GetValueCount();
	if( localIndex < (int)g_ValuesList.size() )
	{
		return g_ValuesList[localIndex];
	}

	return std::string();
}

int SceneComponent::FindValueIndex( std::string name ) const
{
	auto && itr = g_ValuesMap.find( name );
	if( itr != g_ValuesMap.end() )
	{
		return itr->second;
	}

	return me::scene::SceneComponent::FindValueIndex( name );
}

bool SceneComponent::SetValue( int index, std::string value )
{
	if( index < me::scene::SceneComponent::GetValueCount() )
	{
		return me::scene::SceneComponent::SetValue( index, value );
	}

	int localIndex = index - me::scene::SceneComponent::GetValueCount();
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

std::string SceneComponent::GetValue( int index ) const
{
	if( index < me::scene::SceneComponent::GetValueCount() )
	{
		return me::scene::SceneComponent::GetValue( index );
	}

	int localIndex = index - me::scene::SceneComponent::GetValueCount();
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
