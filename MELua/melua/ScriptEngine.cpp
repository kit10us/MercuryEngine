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

MELUADLL_API void ScriptEngine::AddType( Type type )
{
	if ( type.constructor )
	{
		lua_register( m_state, type.name.c_str(), type.constructor ); // Register our type's constructor.
	}

	for (auto named_constructor : type.named_constructors)
	{
		lua_register( m_state, named_constructor.name.c_str(), named_constructor.function ); // Register our type's constructor.
	}

	luaL_newmetatable( m_state, type.name.c_str() ); // Create a metatable by the name of our type on top of the stack.

	if ( type.collector )
	{
		lua_pushcfunction( m_state, type.collector ); 
		lua_setfield( m_state, -2, "__gc" ); // Add a garbage collector to our type (metatable).
	}

	if ( type.newindex )
	{
		lua_pushcfunction( m_state, type.newindex ); 
		lua_setfield( m_state, 1, "__newindex" ); // Add a function to call if we attempt to assign to an unknown member.
	}

	if ( type.index )
	{
		lua_pushcfunction( m_state, type.index ); 
		lua_setfield( m_state, 1, "__index" );
	}
	else
	{
		lua_pushvalue( m_state, -1 );
		lua_setfield( m_state, 1, "__index" );
	}

	if ( type.add )
	{
		lua_pushcfunction( m_state, type.add ); 
		lua_setfield( m_state, 1, "__add" );
	}

	if ( type.sub )
	{
		lua_pushcfunction( m_state, type.sub ); 
		lua_setfield( m_state, 1, "__sub" );
	}

	if ( type.mul )
	{
		lua_pushcfunction( m_state, type.mul ); 
		lua_setfield( m_state, 1, "__mul" );
	}

	if ( type.div )
	{
		lua_pushcfunction( m_state, type.div ); 
		lua_setfield( m_state, 1, "__div" );
	}

	if ( type.mod )
	{
		lua_pushcfunction( m_state, type.mod ); 
		lua_setfield( m_state, 1, "__mod" );
	}

	if ( type.pow )
	{
		lua_pushcfunction( m_state, type.pow ); 
		lua_setfield( m_state, 1, "__pow" );
	}

	if ( type.unm )
	{
		lua_pushcfunction( m_state, type.unm ); 
		lua_setfield( m_state, 1, "__unm" );
	}

	if ( type.concat )
	{
		lua_pushcfunction( m_state, type.concat ); 
		lua_setfield( m_state, 1, "__concat" );
	}

	if ( type.len )
	{
		lua_pushcfunction( m_state, type.len ); 
		lua_setfield( m_state, 1, "__len" );
	}

	if ( type.eq )
	{
		lua_pushcfunction( m_state, type.eq ); 
		lua_setfield( m_state, 1, "__eq" );
	}

	if ( type.lt )
	{
		lua_pushcfunction( m_state, type.lt ); 
		lua_setfield( m_state, 1, "__lt" );
	}

	if ( type.le )
	{
		lua_pushcfunction( m_state, type.le ); 
		lua_setfield( m_state, 1, "__le" );
	}

	lua_pushstring( m_state, type.name.c_str() ); lua_setfield( m_state, -2, "_type" ); // Add a variable called "_type".
	luaL_setfuncs( m_state, type.functions, 0 );
	lua_pop( m_state, 1 );

	m_types[ type.name ] = type;
}

Type * ScriptEngine::GetType( std::string name )
{
	auto itr = m_types.find( name );
	if ( itr == m_types.end() )
	{
		return nullptr;
	}
	else
	{
		return &itr->second;
	}
}

void ScriptEngine::Error( std::string function, std::string message )
{
	m_game->ReportError( me::ErrorLevel::Failure, "LUA", "Failed in \"" + function + "\"! " + message );
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
