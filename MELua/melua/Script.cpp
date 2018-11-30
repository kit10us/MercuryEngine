// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <melua/Script.h>

using namespace melua;

Script::Script( lua_State * state, std::string luaName, unify::Path path )
	: IScript( luaName )
	, m_state{ state }
	, m_luaName{ luaName }
	, m_path{ path }
{
}
		
Script::~Script()
{
	m_state = 0;
}

void Script::GetGlobal( std::string name )
{
	lua_getglobal( m_state, name.c_str() );
}

std::string Script::SetUpValue( int funcindex, int n )
{
	return lua_setupvalue( m_state, funcindex, n );
}

void Script::NewTable()
{
	lua_newtable( m_state );
}

void Script::PushString( std::string s )
{
	lua_pushstring( m_state, s.c_str() );
}

void Script::SetField( int index, std::string k )
{
	lua_setfield( m_state, index, k.c_str() );
}

int Script::GetField( int index, std::string k )
{
	return lua_getfield( m_state, index, k.c_str() );
}

std::string Script::ToString( int index )
{
	return lua_tostring( m_state, index );
}

int Script::SetMetatable( int index )
{
	return lua_setmetatable( m_state, index);
}

int Script::PCall( int nargs, int nresults, int errfunc )
{
	return lua_pcall( m_state, nargs, nresults, errfunc );
}

size_t Script::Owners() const
{
	return IResource::Owners();
}

bool Script::Reload()
{
	auto * se = ScriptEngine::GetInstance(); // Setup the script...	

	int result = luaL_loadfile( m_state, m_path.ToString().c_str() );
	if (result == LUA_ERRSYNTAX)
	{
		Error( m_state, luaL_checkstring( m_state, -1 ) );
	}
	else if (result == LUA_ERRFILE)
	{
		Error( m_state, "Failure trying to read script \"" + m_path.ToString() + "\"!" );
	}
	else if (result != LUA_OK)
	{
		Error( m_state, "Failure in script!" );
	}

	return true;
}

void Script::Pop( int n )
{
	lua_pop( m_state, n );
}

lua_State * Script::GetState()
{
	return m_state;
}

unify::Path Script::GetPath() const
{
	return m_path;
}
