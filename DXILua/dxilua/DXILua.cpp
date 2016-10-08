// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/DXILua.h>
#include <dxilua/ExportCamera.h>
#include <dxilua/ExportDebug.h>
#include <dxilua/ExportGame.h>
#include <dxilua/ExportMatrix.h>
#include <dxilua/ExportObject.h>
#include <dxilua/ExportResources.h>
#include <dxilua/ExportScene.h>
#include <dxilua/ExportScenes.h>
#include <dxilua/ExportUpdate.h>
#include <dxilua/ExportV2.h>
#include <dxilua/ExportV3.h>

#pragma comment( lib, "lua53" )

using namespace dxilua;
using namespace dxi;

ScriptEngine * ScriptEngine::s_se;

class Module : public scripting::IModule
{
public:
	Module( lua_State * state )
		: m_state( state )
	{
	}

	void OnStart() override
	{
		int i = lua_getglobal( m_state, "OnStart" );

		if( i != 0 )
		{
			if( lua_pcall( m_state, 0, 0, 0 ) != 0 )
			{
				assert( 0 ); // TODO:
			}
		}
	}

	void OnUpdate() override
	{
		int i = lua_getglobal( m_state, "OnUpdate" );

		if( i != 0 )
		{
			if( lua_pcall( m_state, 0, 0, 0 ) != 0 )
			{
				assert( 0 ); // TODO:
			}
		}
	}

private: 
	lua_State * m_state;
};

//TODO: Replace asserts with runtime errors.

///////////////////////
///////////////////////

ScriptEngine::ScriptEngine( dxi::core::Game * game )
	: m_game( game ),
	m_state{ luaL_newstate() }
{
	s_se = this;

	luaL_openlibs( m_state );

	// Add custom functions...
	ExportGame( m_state );
	ExportResources( m_state );
	ExportScenes( m_state );
	
	//ExportScene( m_state );
	RegisterScene( m_state );

	//ExportObject( m_state );
	RegisterObject( m_state );

	ExportCamera( m_state );
	ExportMatrix( m_state );
	ExportDebug( m_state );
	ExportUpdate( m_state );
	ExportV2( m_state );
	ExportV3( m_state );
}

ScriptEngine::~ScriptEngine()
{
	if ( m_state != 0 )
	{
		lua_close( m_state );
	}
}


scripting::ExecuteResult ScriptEngine::ExecuteString( std::string line )
{
	int result = luaL_loadstring( m_state, line.c_str() );
	if( result != LUA_OK )
	{
		//print_error( state );
		return scripting::ExecuteResult::Fail;
	}

	result = lua_pcall( m_state, 0, LUA_MULTRET, 0 );
	if( result != LUA_OK )
	{
		//print_error( state );
		return scripting::ExecuteResult::Fail;
	}

	return scripting::ExecuteResult::Pass;
}

scripting::ExecuteResult ScriptEngine::ExecuteFile( unify::Path path )
{
	int result = luaL_loadfile( m_state, path.ToString().c_str() );
	if( result != LUA_OK )
	{
		//print_error( state );
		return scripting::ExecuteResult::Fail;
	}

	result = lua_pcall( m_state, 0, LUA_MULTRET, 0 );
	if( result != LUA_OK )
	{
		//print_error( state );
		return scripting::ExecuteResult::Fail;
	}

	return scripting::ExecuteResult::Pass;
}

scripting::IModule::ptr ScriptEngine::LoadModule( unify::Path path )
{
	int result = luaL_loadfile( m_state, path.ToString().c_str() );
	if ( result == LUA_ERRSYNTAX )
	{
		m_game->ReportError( dxi::ErrorLevel::Failure, "Lua", luaL_checkstring( m_state, -1 ) );
		return scripting::IModule::ptr();
	}
	else if( result != LUA_OK )
	{
		return scripting::IModule::ptr();
	}

	scripting::IModule::ptr module( new Module( m_state ) );

	result = lua_pcall( m_state, 0, LUA_MULTRET, 0 );
	if( result != LUA_OK )
	{
		return scripting::IModule::ptr();
	}

	return module;
}

dxi::core::Game * ScriptEngine::GetGame()
{
	return ScriptEngine::s_se->m_game;
}