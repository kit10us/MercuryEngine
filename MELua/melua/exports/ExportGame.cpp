// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <melua/exports/ExportGame.h>
#include <melua/ScriptEngine.h>

using namespace melua;
using namespace me;

int Game_GetWidth( lua_State * state )
{
	int args = lua_gettop( state );
	int renderer = 0;
	if ( args == 1 )
	{
		renderer = (int)lua_tonumber( state, 1 );
	}

	ScriptEngine * se = ScriptEngine::GetInstance();
	auto game = se->GetGame();

	lua_pushnumber( state, game->GetOS()->GetRenderer( renderer )->GetViewport().GetSize().width );

	return 1;
}

int Game_GetHeight( lua_State * state )
{
	int args = lua_gettop( state );
	int renderer = 0;
	if( args == 1 )
	{
		renderer = (int)lua_tonumber( state, 1 );
	}

	ScriptEngine * se = ScriptEngine::GetInstance();
	auto game = se->GetGame();

	lua_pushnumber( state, game->GetOS()->GetRenderer( renderer )->GetViewport().GetSize().height );

	return 1;
}

int Game_GetRendererCount( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 0 );

	ScriptEngine * se = ScriptEngine::GetInstance();
	auto game = se->GetGame();

	float result = (float)game->GetOS()->RendererCount();

	lua_pushnumber( state, result );

	return 1;
}

int Game_Quit( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 0 );

	ScriptEngine * se = ScriptEngine::GetInstance();
	auto game = se->GetGame();

	game->Quit();

	return 0;
}

int Game_Command( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	std::string command = lua_tostring( state, 1 );			  
	ScriptEngine * se = ScriptEngine::GetInstance();
	auto game = se->GetGame();
	game->Feed( "COMMAND", (char*)command.c_str() );

	return 0;
}

static const luaL_Reg gameFuncs[] =
{
	{ "GetWidth", Game_GetWidth },
	{ "GetHeight", Game_GetHeight },
	{ "GetRendererCount", Game_GetRendererCount },
	{ "Quit", Game_Quit },
	{ "Command", Game_Command },
	{ nullptr, nullptr }
};

int ExportGame( lua_State * state )
{
	luaL_newlib( state, gameFuncs );
	lua_setglobal( state, "Game" );
	return 1;
}
