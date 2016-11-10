// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <DXILuaDLL.h>
#include <me/IGame.h>
#include <me/IScriptEngine.h>
#include <lua.hpp>

namespace dxilua
{
	class ScriptEngine : public me::IScriptEngine
	{
	public:
		ScriptEngine( me::IGame * game );
		~ScriptEngine();

		std::string GetName() const override;

		void OnAttach( me::IGame * game ) override;

		void OnUpdate( me::IGame * game, const me::RenderInfo & renderInfo ) override;

		void OnRender( me::IGame * game, const me::RenderInfo & renderInfo ) override;

		void OnDetach( me::IGame * game ) override;

		me::ExecuteResult ExecuteString( std::string line ) override;
		me::ExecuteResult ExecuteFile( unify::Path path ) override;

		me::scene::IObjectComponent::ptr LoadModule( unify::Path path ) override;

		DXILUADLL_API lua_State * GetState();

		DXILUADLL_API void AddLibrary( const char * group, const luaL_Reg * list, int count );

		DXILUADLL_API void AddType( const char * name, const luaL_Reg * functions, int count, lua_CFunction constructor, lua_CFunction collector );



		static me::IGame * GetGame();

	private:
		me::IGame * m_game;		
		lua_State * m_state;
		size_t m_moduleCount;

		static ScriptEngine * s_se;
	};
}