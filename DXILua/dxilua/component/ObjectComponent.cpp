// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/Util.h>
#include <dxilua/component/ObjectComponent.h>
#include <dxilua/ExportObject.h>
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

ObjectComponent::ObjectComponent( ObjectComponent & component )
	: ObjectComponent( component.m_state, component.m_game, component.m_luaName, component.m_path  )
{
}

ObjectComponent::ObjectComponent( lua_State * state, me::IGame * game, std::string luaName, unify::Path path )
	: me::object::ObjectComponent( "LUAScript" )
	, m_state( state )
	, m_game( game )
	, m_path( path )
{
}

ObjectComponent::~ObjectComponent()
{
	m_state = 0;
}

std::string ObjectComponent::GetLuaName() const
{
	return m_luaName;
}

void ObjectComponent::CallMember( std::string function )
{
	// Get our _ENV...
	if ( !lua_getfield( m_state, LUA_REGISTRYINDEX, m_luaName.c_str() ) )						   
	{
		m_game->ReportError( me::ErrorLevel::Failure, "LUA", "Object not found in lua module! (" + m_luaName + ")" );
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
			 
void ObjectComponent::OnInit()
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

void ObjectComponent::OnStart()
{
	CallMember( "OnStart" );
}

void ObjectComponent::OnUpdate( me::UpdateParams params )
{
	CallMember( "OnUpdate" );
}

void ObjectComponent::CollectGeometry( me::GeometryCache & cache, const unify::FrameLite * frame )
{																										
	// DO NOTHING
}

void ObjectComponent::OnSuspend()
{
	CallMember( "OnSuspend" );
}

void ObjectComponent::OnResume()
{
	CallMember( "OnResume" );
}

me::object::IObjectComponent::ptr ObjectComponent::Duplicate()
{
	auto duplicate = new ObjectComponent( *this );
	return me::object::IObjectComponent::ptr( duplicate );
}

std::string ObjectComponent::GetWhat() const
{
	return m_path.Filename();
}

int ObjectComponent::GetValueCount() const
{
	return (int)g_ValuesList.size();
}

bool ObjectComponent::ValueExists( std::string name ) const
{
	auto && itr = g_ValuesMap.find( name );
	if ( itr == g_ValuesMap.end() )
	{
		return false;
	}
	else
	{
		return true;
	}
}

std::string ObjectComponent::GetValueName( int index ) const
{
	if ( index >= (int)g_ValuesList.size() )
	{
		return std::string();
	}
	else
	{
		return g_ValuesList[ index ];
	}
}

int ObjectComponent::FindValueIndex( std::string name ) const
{
	auto && itr = g_ValuesMap.find( name );
	if ( itr == g_ValuesMap.end() )
	{
		return -1;
	}
	else
	{
		return itr->second;
	}
}

bool ObjectComponent::SetValue( int index, std::string value )
{
	switch ( index )
	{
	default:
		return false;
	case 0:
		return false;
	}
	return true;
}

bool ObjectComponent::SetValue( std::string name, std::string value )
{
	int index = FindValueIndex( name );
	return SetValue( index, value );
}

std::string ObjectComponent::GetValue( int index ) const
{
	switch ( index )
	{
	default:
		return std::string();
	case 0:
		return m_path.ToString();
	}
}
 
std::string ObjectComponent::GetValue( std::string name ) const
{
	int index = FindValueIndex( name );
	return GetValue( index );
}
			  
