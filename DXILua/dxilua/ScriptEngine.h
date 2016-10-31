// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <DXILuaDLL.h>
#include <dxi/core/IGame.h>
#include <dxi/scripting/IScriptEngine.h>
#include <lua.hpp>

namespace dxilua
{
	class ScriptEngine : public dxi::scripting::IScriptEngine
	{
	public:
		ScriptEngine( dxi::core::IGame * game );
		~ScriptEngine();

		std::string GetName() const override;

		void OnAttach( dxi::core::IGame * game ) override;

		void OnUpdate( dxi::core::IGame * game, const dxi::RenderInfo & renderInfo ) override;

		void OnRender( dxi::core::IGame * game, const dxi::RenderInfo & renderInfo ) override;

		void OnDetach( dxi::core::IGame * game ) override;

		dxi::scripting::ExecuteResult ExecuteString( std::string line ) override;
		dxi::scripting::ExecuteResult ExecuteFile( unify::Path path ) override;

		dxi::scene::IObjectComponent::ptr LoadModule( unify::Path path ) override;

		DXILUADLL_API lua_State * GetState();

		DXILUADLL_API void AddLibrary( const char * group, const luaL_Reg * list, int count );

		DXILUADLL_API void AddType( const char * name, const luaL_Reg * functions, int count, lua_CFunction constructor, lua_CFunction collector );



		static dxi::core::IGame * GetGame();

	private:
		dxi::core::IGame * m_game;		
		lua_State * m_state;
		size_t m_moduleCount;

		static ScriptEngine * s_se;
	};
}