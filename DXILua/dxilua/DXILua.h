// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

//#pragma comment( lib, "DXILua" )

#include <dxi/core/Game.h>
#include <dxi/scripting/IScriptEngine.h>
#include <lua.hpp>

namespace dxilua
{
	class ScriptEngine : public dxi::scripting::IScriptEngine
	{
	public:
		ScriptEngine( dxi::core::Game & game );
		~ScriptEngine();

		dxi::scripting::ExecuteResult ExecuteString( std::string line ) override;
		dxi::scripting::ExecuteResult ExecuteFile( unify::Path path ) override;

		static dxi::core::Game & GetGame();

	private:
		dxi::core::Game & m_game;		
		lua_State * m_state;

		static ScriptEngine * s_se;
	};
}