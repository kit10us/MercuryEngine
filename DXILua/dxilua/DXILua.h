// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <DXILuaDLL.h>
#include <dxi/core/Game.h>
#include <dxi/scripting/IScriptEngine.h>
#include <lua.hpp>

namespace dxilua
{

	class ILuaExtension
	{
	public:
		virtual ~ILuaExtension()
		{
		}
	};


	class ScriptEngine : public dxi::scripting::IScriptEngine
	{
	public:
		ScriptEngine( dxi::core::Game * game );
		~ScriptEngine();

		dxi::scripting::ExecuteResult ExecuteString( std::string line ) override;
		dxi::scripting::ExecuteResult ExecuteFile( unify::Path path ) override;

		dxi::scripting::IModule::ptr LoadModule( unify::Path path, dxi::scene::Object::ptr object ) override;

		DXILUADLL_API lua_State * GetState();

		DXILUADLL_API void AddLibrary( const char * group, const luaL_Reg * list, int count );

		DXILUADLL_API void AddType( const char * name, const luaL_Reg * functions, int count, lua_CFunction constructor, lua_CFunction collector );

		static dxi::core::Game * GetGame();

	private:
		dxi::core::Game * m_game;		
		lua_State * m_state;

		std::list< dxi::scripting::IModule::ptr > m_modules;

		static ScriptEngine * s_se;
	};
}