// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/DXILua.h>
#include <dxilua/Module.h>
#include <dxilua/CreateState.h>

#pragma comment( lib, "lua53" )

using namespace dxilua;
using namespace dxi;

ScriptEngine * ScriptEngine::s_se;

ScriptEngine::ScriptEngine( dxi::core::Game * game )
	: m_game( game )
	//, m_state{ CreateState() }
{
	s_se = this;
}

ScriptEngine::~ScriptEngine()
{
	/*
	if ( m_state != 0 )
	{
		lua_close( m_state );
	}
	*/
}


scripting::ExecuteResult ScriptEngine::ExecuteString( std::string line )
{
	/*
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
	*/

	return scripting::ExecuteResult::Pass;
}

scripting::ExecuteResult ScriptEngine::ExecuteFile( unify::Path path )
{
	/*
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
	*/

	return scripting::ExecuteResult::Pass;
}

scripting::IModule::ptr ScriptEngine::LoadModule( unify::Path path, dxi::scene::Object::ptr object )
{
	lua_State * state = CreateState();
	int result = luaL_loadfile( state, path.ToString().c_str() );
	if ( result == LUA_ERRSYNTAX )
	{
		m_game->ReportError( dxi::ErrorLevel::Failure, "Lua", luaL_checkstring( state, -1 ) );
		return scripting::IModule::ptr();
	}
	else if( result != LUA_OK )
	{
		return scripting::IModule::ptr();
	}

	scripting::IModule::ptr module( new Module( state, m_game ) );

	result = lua_pcall( state, 0, LUA_MULTRET, 0 );
	if( result != LUA_OK )
	{
		return scripting::IModule::ptr();
	}

	module->BindToObject( object );

	return module;
}

dxi::core::Game * ScriptEngine::GetGame()
{
	return ScriptEngine::s_se->m_game;
}