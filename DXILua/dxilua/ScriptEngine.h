// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <DXILuaDLL.h>
#include <me/GameComponent.h>
#include <me/scene/Scene.h>
#include <lua.hpp>

namespace dxilua
{
	enum class ExecuteResult
	{
		Pass,
		Fail,
		Pending
	};

	class ScriptEngine : public me::GameComponent
	{
	public:
		ScriptEngine();
		~ScriptEngine();

		ExecuteResult ExecuteString( std::string line );
		ExecuteResult ExecuteFile( unify::Path path );

		me::IGameComponent::ptr LoadGameScript( unify::Path path );

		me::scene::IObjectComponent::ptr LoadObjectScript( unify::Path path );

		DXILUADLL_API lua_State * GetState();

		DXILUADLL_API void AddLibrary( const char * group, const luaL_Reg * list, int count );

		DXILUADLL_API void AddType( const char * name, const luaL_Reg * functions, int count, lua_CFunction constructor, lua_CFunction collector );

		static me::IGame * GetGame();

	public: // IComponent...
		std::string GetWhat() const override;
		
	private:
		lua_State * m_state;
		size_t m_gameScriptCount;
		size_t m_objectScriptCount;

		static ScriptEngine * s_se;
	};
}