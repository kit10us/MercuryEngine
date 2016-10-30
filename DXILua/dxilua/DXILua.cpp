// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/Util.h>
#include <dxilua/DXILua.h>
#include <dxilua/Module.h>
#include <dxilua/CreateState.h>
#include <dxilua/ExportObject.h>

#pragma comment( lib, "lua53" )

using namespace dxilua;
using namespace dxi;

ScriptEngine * ScriptEngine::s_se;

ScriptEngine::ScriptEngine( dxi::core::IGame * game )
	: m_game( game )
	, m_state{ CreateState() }
{
	s_se = this;
}

ScriptEngine::~ScriptEngine()
{
	if ( m_state != 0 )
	{
		lua_close( m_state );
	}
}

std::string ScriptEngine::GetName() const
{
	return "LUA";
}

void ScriptEngine::OnAttach( dxi::core::IGame * game )
{
}

void ScriptEngine::OnUpdate( dxi::core::IGame * game, const dxi::RenderInfo & renderInfo )
{
}

void ScriptEngine::OnRender( dxi::core::IGame * game, const dxi::RenderInfo & renderInfo )
{
}

void ScriptEngine::OnDetach( dxi::core::IGame * game )
{
}

scripting::ExecuteResult ScriptEngine::ExecuteString( std::string line )
{
	int result = luaL_loadstring( m_state, line.c_str() );
	if( result != LUA_OK )
	{
		return scripting::ExecuteResult::Fail;
	}

	result = lua_pcall( m_state, 0, LUA_MULTRET, 0 );
	if( result != LUA_OK )
	{
		return scripting::ExecuteResult::Fail;
	}

	return scripting::ExecuteResult::Pass;
}

scripting::ExecuteResult ScriptEngine::ExecuteFile( unify::Path path )
{
	int result = luaL_loadfile( m_state, path.ToString().c_str() );
	if( result != LUA_OK )
	{
		return scripting::ExecuteResult::Fail;
	}

	result = lua_pcall( m_state, 0, LUA_MULTRET, 0 );
	if( result != LUA_OK )
	{
		return scripting::ExecuteResult::Fail;
	}

	return scripting::ExecuteResult::Pass;
}

scripting::IModule::ptr ScriptEngine::LoadModule( unify::Path path, dxi::scene::Object::ptr object )
{					
	int top = lua_gettop( m_state );

	std::string name = "__" + path.FilenameNoExtension() + "_" + unify::Cast< std::string >( m_modules.size() );

	lua_State * state = m_state;

	scripting::IModule::ptr module( new Module( object, m_state, m_game, name, path ) );
	m_modules.push_back( module );

	return module;
}

lua_State * ScriptEngine::GetState()
{
	return m_state;
}
												 
DXILUADLL_API void ScriptEngine::AddLibrary( const char * group, const luaL_Reg * list, int count )
{
	luaL_checkversion( m_state );
	lua_createtable( m_state, 0, 1 );
	luaL_setfuncs( m_state, list, 0 );

	lua_setglobal( m_state, group );
}

DXILUADLL_API void ScriptEngine::AddType( const char * name, const luaL_Reg * functions, int count, lua_CFunction constructor, lua_CFunction collector )
{
	lua_register( m_state, name, constructor );
	luaL_newmetatable( m_state, name );
	lua_pushcfunction( m_state, collector ); lua_setfield( m_state, -2, "__gc" );
	lua_pushvalue( m_state, -1 ); lua_setfield( m_state, -2, "__index" );
	luaL_setfuncs( m_state, functions, 0 );
	lua_pop( m_state, 1 );
}

dxi::core::IGame * ScriptEngine::GetGame()
{
	return ScriptEngine::s_se->m_game;
}