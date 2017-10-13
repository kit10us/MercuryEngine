// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <melua/FunctionAction.h>
#include <melua/ScriptEngine.h>

using namespace melua;

FunctionAction::FunctionAction( int refIndex )
	: m_refIndex{ refIndex }
{
}

bool FunctionAction::Perform()
{
	ScriptEngine * se = ScriptEngine::GetInstance();
	me::Game * game = se->GetGame();
	auto state = se->GetState();

	lua_rawgeti( state, LUA_REGISTRYINDEX, m_refIndex );
	std::string t2 = GetTypename( state, -1 );
	lua_pcall( state, 0, 0, 0 );

 	return true;
}