// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <melua/Util.h>
#include <melua/ScriptEngine.h>
#include <melua/component/GameComponent.h>
#include <melua/component/SceneComponent.h>
#include <melua/component/ObjectComponent.h>
#include <melua/CreateState.h>
#include <melua/ExportObject.h>

#pragma comment( lib, "lua53" )

using namespace melua;
using namespace me;
using namespace scene;
using namespace object;

void GameComponentDeleter( me::IGameComponent * gc )
{
	delete gc;
}


ScriptEngine * ScriptEngine::s_se;

ScriptEngine::ScriptEngine( me::IOS * os )
	: GameComponent( "Lua" )
	, m_state{ luaL_newstate() }
	, m_gameScriptCount{ 0 }
	, m_objectScriptCount{ 0 }
{
	s_se = this;
	RegisterLibraries( m_state );

	// Set path...
	std::string path = unify::StringReplace( os->GetAssetPaths().GetPaths( os->GetRunPath() ), ";", "?.lua;" );

    lua_getglobal( m_state, "package" );
    lua_getfield( m_state, -1, "path" ); // get field "path" from table at top of stack (-1)
    std::string cur_path = lua_tostring( m_state, -1 ); // grab path string from top of stack
    cur_path.append( ";" ); // do your path magic here
    cur_path.append( path );
    lua_pop( m_state, 1 ); // get rid of the string on the stack we just pushed on line 5
    lua_pushstring( m_state, cur_path.c_str() ); // push the new one
    lua_setfield( m_state, -2, "path" ); // set the field "path" in table at -2 with value at top of stack
    lua_pop( m_state, 1 ); // get rid of package table from top of stack
}

ScriptEngine::~ScriptEngine()
{
	if ( m_state != 0 )
	{
		lua_close( m_state );
	}
}

ExecuteResult ScriptEngine::ExecuteString( std::string line )
{
	int result = luaL_loadstring( m_state, line.c_str() );
	if( result != LUA_OK )
	{
		return ExecuteResult::Fail;
	}

	result = lua_pcall( m_state, 0, LUA_MULTRET, 0 );
	if( result != LUA_OK )
	{
		return ExecuteResult::Fail;
	}

	return ExecuteResult::Pass;
}

ExecuteResult ScriptEngine::ExecuteFile( unify::Path path )
{
	path = m_game->GetOS()->GetAssetPaths().FindAsset( path );

	int result = luaL_loadfile( m_state, path.ToString().c_str() );
	if( result != LUA_OK )
	{
		return ExecuteResult::Fail;
	}

	result = lua_pcall( m_state, 0, LUA_MULTRET, 0 );
	if( result != LUA_OK )
	{
		return ExecuteResult::Fail;
	}

	return ExecuteResult::Pass;
}

IGameComponent::ptr ScriptEngine::LoadGameScript( unify::Path path )
{					
	path = m_game->GetOS()->GetAssetPaths().FindAsset( path );

	int top = lua_gettop( m_state );

	std::string luaName = "__" + path.FilenameNoExtension() + "_" + unify::Cast< std::string >( m_gameScriptCount++ );

	IGameComponent::ptr module( new component::GameComponent( m_state, luaName, path ), GameComponentDeleter );
	return module;
}

ISceneComponent::ptr ScriptEngine::LoadSceneScript( unify::Path path )
{					
	path = m_game->GetOS()->GetAssetPaths().FindAsset( path );

	int top = lua_gettop( m_state );

	std::string luaName = "__" + path.FilenameNoExtension() + "_" + unify::Cast< std::string >( m_objectScriptCount++ );

	lua_State * state = m_state;

	ISceneComponent::ptr module( new component::SceneComponent( m_game, m_state, luaName, path ) );

	return module;
}

IObjectComponent::ptr ScriptEngine::LoadObjectScript( unify::Path path )
{					
	path = m_game->GetOS()->GetAssetPaths().FindAsset( path );

	int top = lua_gettop( m_state );

	std::string luaName = "__" + path.FilenameNoExtension() + "_" + unify::Cast< std::string >( m_objectScriptCount++ );

	lua_State * state = m_state;

	IObjectComponent::ptr module( new component::ObjectComponent( m_game, m_state, luaName, path ) );

	return module;
}

lua_State * ScriptEngine::GetState()
{
	return m_state;
}
												 
MELUADLL_API void ScriptEngine::AddLibrary( const char * group, const luaL_Reg * list, int count )
{
	luaL_checkversion( m_state );
	lua_createtable( m_state, 0, 1 );
	luaL_setfuncs( m_state, list, 0 );

	lua_setglobal( m_state, group );
}

MELUADLL_API void ScriptEngine::AddType( const char * name, const luaL_Reg * functions, int count, lua_CFunction constructor, lua_CFunction collector )
{
	lua_register( m_state, name, constructor );
	luaL_newmetatable( m_state, name );
	lua_pushcfunction( m_state, collector ); lua_setfield( m_state, -2, "__gc" );
	lua_pushvalue( m_state, -1 ); lua_setfield( m_state, -2, "__index" );
	lua_pushstring( m_state, name ); lua_setfield( m_state, -2, "_type" );
	luaL_setfuncs( m_state, functions, 0 );
	lua_pop( m_state, 1 );
}

ScriptEngine* ScriptEngine::GetInstance()
{
	return s_se;
}

me::IGame * ScriptEngine::GetGame()
{
	return s_se->GameComponent::GetGame();
}

std::string ScriptEngine::GetWhat() const
{
	return std::string();
}
