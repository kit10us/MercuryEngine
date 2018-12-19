// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <melua/Util.h>
#include <melua/component/ObjectComponent.h>
#include <melua/exports/ExportObject.h>
#include <melua/CreateState.h>
#include <me/interop/MyThing.h>

using namespace melua;
using namespace component;

ObjectComponent::ObjectComponent( ObjectComponent & component )
	: ObjectComponent( component.m_game, component.m_state, component.m_luaName, component.m_path  )
{
	using namespace me;
	using namespace interop;

	GetLookup()->Add( "luaName",
		me::interop::IValue::ptr{ new interop::MyThing< ObjectComponent * >(
			this,
			[&]( const ObjectComponent * component ) -> std::string { return component->GetLuaName(); }
			)
	} );

	GetLookup()->Add( "luaPath",
		me::interop::IValue::ptr{ new interop::MyThing< ObjectComponent * >(
			this,
			[&]( const ObjectComponent * component ) -> std::string { return component->GetPath().ToString(); }
			)
	} );
}

ObjectComponent::ObjectComponent( me::game::IGame * gameInstance, lua_State * state, std::string luaName, unify::Path path )
	: me::object::component::ObjectComponent( "LUAScript", false, false )
	, m_state( state )
	, m_game( gameInstance )
	, m_path( path )
{
	using namespace me;
	using namespace interop;

	GetLookup()->Add( "luaName",
		me::interop::IValue::ptr{ new interop::MyThing< ObjectComponent * >(
			this,
			[&]( const ObjectComponent * component ) -> std::string { return component->GetLuaName(); }
			)
	} );

	GetLookup()->Add( "path",
		me::interop::IValue::ptr{ new interop::MyThing< ObjectComponent * >(
			this,
			[&]( const ObjectComponent * component ) -> std::string { return component->GetPath().ToString(); }
			)
	} );
}

ObjectComponent::~ObjectComponent()
{
	m_state = 0;
}

std::string ObjectComponent::GetLuaName() const
{
	return m_luaName;
}

unify::Path ObjectComponent::GetPath() const
{
	return m_path;
}

void ObjectComponent::CallMember( std::string function )
{
	// Get our _ENV...
	if ( !lua_getfield( m_state, LUA_REGISTRYINDEX, m_luaName.c_str() ) )						   
	{
		Error( m_state, "Object not found in lua module! (" + m_luaName + ")" );
	}

	int r2 = lua_getfield( m_state, -1, function.c_str() );

	if ( r2 != 0 )
	{
		if ( lua_pcall( m_state, 0, 0, 0 ) != 0 )
		{
			std::string error = lua_tostring( m_state, -1 );
			Error( m_state, "Failed in script " + function + ": " + error );
		}
	}
	else
	{
		lua_pop( m_state, 1 );
	}

	// Pop our _ENV.
	lua_pop( m_state, 1 );
}
			 
void ObjectComponent::OnStart()
{
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

	if ( m_object )
	{
		PushUserType< ObjectProxy >( m_state, { m_object } );
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
		Error( m_state,"Failed with script initial call: " + error );
	}

	CallMember( "OnStart" );
}

void ObjectComponent::OnUpdate( const me::UpdateParams & params )
{
	CallMember( "OnUpdate" );
}

void ObjectComponent::OnSuspend()
{
	CallMember( "OnSuspend" );
}

void ObjectComponent::OnResume()
{
	CallMember( "OnResume" );
}

me::object::component::IObjectComponent::ptr ObjectComponent::Duplicate()
{
	auto duplicate = new ObjectComponent( *this );
	return me::object::component::IObjectComponent::ptr( duplicate );
}

std::string ObjectComponent::GetWhat() const
{
	return m_path.Filename();
}