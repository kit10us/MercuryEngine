// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/Util.h>
#include <dxilua/Module.h>
#include <dxilua/ExportObject.h>
#include <dxilua/CreateState.h>

using namespace dxilua;
using namespace me;
using namespace scene;

Module::Module( Module & component )
	: m_state( component.m_state )
	, m_game( component.m_game )
	, m_name( component.m_name )
	, m_path( component.m_path )
	, m_enabled( component.m_enabled )
{
}

Module::Module( lua_State * state, IGame * game, std::string name, unify::Path path )
	: m_state( state )
	, m_game( game )
	, m_name( name )
	, m_path( path )
	, m_enabled( true )
{
}

Module::~Module()
{
	m_state = 0;
}

std::string Module::GetName() const
{
	return m_name;
}

bool Module::IsEnabled() const
{
	return m_enabled;
}

void Module::SetEnabled( bool enabled )
{
	m_enabled = enabled;
}

void Module::CallMember( std::string function )
{
	// Get our _ENV...
	if ( !lua_getfield( m_state, LUA_REGISTRYINDEX, m_name.c_str() ) )						   
	{
		m_game->ReportError( ErrorLevel::Failure, "LUA", "Module not found! (" + m_name + ")" );
	}

	int r2 = lua_getfield( m_state, -1, function.c_str() );

	if ( r2 != 0 )
	{
		if ( lua_pcall( m_state, 0, 0, 0 ) != 0 )
		{
			std::string error = lua_tostring( m_state, -1 );
			m_game->ReportError( ErrorLevel::Failure, "LUA", "Failed in script " + function + ": " + error );
		}
	}
	else
	{
		lua_pop( m_state, 1 );
	}

	// Pop our _ENV.
	lua_pop( m_state, 1 );
}

void Module::OnAttach( Object * object )
{
	m_object = object;
}

void Module::OnDetach()
{
	m_object = nullptr;
}
			 
void Module::OnInit()
{
	// Setup the script...	
	int result = luaL_loadfile( m_state, m_path.ToString().c_str() );
	if ( result == LUA_ERRSYNTAX )
	{
		m_game->ReportError( ErrorLevel::Failure, "Lua", luaL_checkstring( m_state, -1 ) );
		assert( 0 );
	}
	else if ( result == LUA_ERRFILE )
	{
		m_game->ReportError( ErrorLevel::Failure, "Lua", "Failure trying to read script \"" + m_path.ToString() + "\"!" );
	}
	else if ( result != LUA_OK )
	{
		m_game->ReportError( ErrorLevel::Failure, "Lua", "Failure in script!" );
	}

	// Create table for modules _ENV table.
	lua_newtable( m_state );

	// Add member variables.
	lua_pushstring( m_state, m_name.c_str() );
	lua_setfield( m_state, -2, "_name" );

	if ( m_object )
	{
		PushObject( m_state, m_object );
	}
	else
	{
		lua_pushnil( m_state );
	}
	lua_setfield( m_state, -2, "_object" );

	// Create new metatable for __index to be _G (so missed functions, non-member functions, look in _G).
	lua_newtable( m_state );
	lua_getglobal( m_state, "_G" );
	lua_setfield( m_state, -2, "__index" );
	lua_setmetatable( m_state, -2 ); // Set global as the metatable

	// Push to registery with a unique name.
	lua_setfield( m_state, LUA_REGISTRYINDEX, m_name.c_str() );

	// Retrieve registry.
	int i = lua_getfield( m_state, LUA_REGISTRYINDEX, m_name.c_str() );

	// Set the upvalue (_ENV)
	const char * uv = lua_setupvalue( m_state, -2, 1 );

	result = lua_pcall( m_state, 0, LUA_MULTRET, 0 );
	if ( result != LUA_OK )
	{
		std::string error = lua_tostring( m_state, -1 );
		m_game->ReportError( ErrorLevel::Failure, "LUA", "Failed with script initial call: " + error );
		assert( 0 ); // TODO:
	}

	CallMember( "OnInit" );
}

void Module::OnStart()
{
	CallMember( "OnStart" );
}

void Module::OnUpdate( IRenderer * renderer, const RenderInfo & renderInfo )
{
	CallMember( "OnUpdate" );
}

void Module::CollectGeometry( GeometryCache & cache, const unify::FrameLite * frame )
{																										
	// DO NOTHING
}

void Module::OnSuspend()
{
	CallMember( "OnSuspend" );
}

void Module::OnResume()
{
	CallMember( "OnResume" );
}

IObjectComponent * Module::Duplicate()
{
	auto duplicate = new Module( *this );
	return duplicate;
}
