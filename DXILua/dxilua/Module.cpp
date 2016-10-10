// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/Module.h>
#include <dxilua/ExportObject.h>
#include <dxilua/CreateState.h>

using namespace dxilua;
using namespace dxi;

Module::Module( lua_State * state, dxi::core::Game * game )
	: m_state( state )
	, m_game( game )
{
}

Module::~Module()
{
	lua_close( m_state );
	m_state = 0;
}

void Module::BindToObject( scene::Object::ptr object )
{
	m_object = object;
}

void Module::OnStart()
{
	int i = lua_getglobal( m_state, "OnStart" );

	if( i != 0 )
	{
		if ( m_object )
		{
			ObjectProxy ** childProxy = (ObjectProxy**)(lua_newuserdata( m_state, sizeof( ObjectProxy* ) ));
			*childProxy = new ObjectProxy;
			luaL_setmetatable( m_state, "Object" );
			(*childProxy)->object = m_object;
		}
		else
		{
			lua_pushnil( m_state );
		}
	
		if( lua_pcall( m_state, 1, 0, 0 ) != 0 )
		{
			std::string error = lua_tostring( m_state, 1 );									    
			m_game->ReportError( dxi::ErrorLevel::Failure, "LUA", "Failed with script OnStart: " + error  );
			assert( 0 ); // TODO:
		}
	}
}

void Module::OnUpdate()
{
	int i = lua_getglobal( m_state, "OnUpdate" );
				  
	if( i != 0 )
	{
		int a = lua_gettop( m_state );
		if( m_object )
		{
			ObjectProxy ** childProxy = (ObjectProxy**)(lua_newuserdata( m_state, sizeof( ObjectProxy* ) ));
			*childProxy = new ObjectProxy;
			luaL_setmetatable( m_state, "Object" );
			(*childProxy)->object = m_object;
		}
		else
		{
			lua_pushnil( m_state );
		}

		if( lua_pcall( m_state, 1, 0, 0 ) != 0 )
		{
			std::string error = lua_tostring( m_state, 1 );
			m_game->ReportError( dxi::ErrorLevel::Failure, "LUA", "Failed with script OnUpdate: " + error );
			assert( 0 ); // TODO:
		}
		int b = lua_gettop( m_state );
		int c = b - a;
	}
}

void Module::OnSuspend()
{
	int i = lua_getglobal( m_state, "OnSuspend" );

	if( i != 0 )
	{
		if( m_object )
		{
			ObjectProxy ** childProxy = (ObjectProxy**)(lua_newuserdata( m_state, sizeof( ObjectProxy* ) ));
			*childProxy = new ObjectProxy;
			luaL_setmetatable( m_state, "Object" );
			(*childProxy)->object = m_object;
		}
		else
		{
			lua_pushnil( m_state );
		}

		if( lua_pcall( m_state, 1, 0, 0 ) != 0 )
		{
			std::string error = lua_tostring( m_state, 1 );
			m_game->ReportError( dxi::ErrorLevel::Failure, "LUA", "Failed with script OnSuspend: " + error );
			assert( 0 ); // TODO:
		}
	}
}

void Module::OnResume()
{
	int i = lua_getglobal( m_state, "OnResume" );

	if( i != 0 )
	{
		if( m_object )
		{
			ObjectProxy ** childProxy = (ObjectProxy**)(lua_newuserdata( m_state, sizeof( ObjectProxy* ) ));
			*childProxy = new ObjectProxy;
			luaL_setmetatable( m_state, "Object" );
			(*childProxy)->object = m_object;
		}
		else
		{
			lua_pushnil( m_state );
		}

		if( lua_pcall( m_state, 1, 0, 0 ) != 0 )
		{
			std::string error = lua_tostring( m_state, 1 );
			m_game->ReportError( dxi::ErrorLevel::Failure, "LUA", "Failed with script OnResume: " + error );
			assert( 0 ); // TODO:
		}
	}
}