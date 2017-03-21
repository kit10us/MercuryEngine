// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <MELuaDLL.h>
#include <me/GameComponent.h>
#include <me/scene/Scene.h>
#include <melua/Type.h>
#include <lua.hpp>

namespace melua
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
		ScriptEngine( me::IOS * os );
		~ScriptEngine();

		ExecuteResult ExecuteString( std::string line );
		ExecuteResult ExecuteFile( unify::Path path );

		me::IGameComponent::ptr LoadGameScript( unify::Path path );

		me::object::IObjectComponent::ptr LoadObjectScript( unify::Path path );

		me::scene::ISceneComponent::ptr LoadSceneScript( unify::Path path );

		MELUADLL_API lua_State * GetState();

		MELUADLL_API void AddLibrary( const char * group, const luaL_Reg * list, int count );

		MELUADLL_API void AddType( Type type );

		Type * GetType( std::string name );

		void Error( std::string function, std::string message );

		static ScriptEngine* GetInstance();
		static me::Game * GetGame();

	public: // IComponent...
		std::string GetWhat() const override;
		
	private:
		lua_State * m_state;
		size_t m_gameScriptCount;
		size_t m_objectScriptCount;

		std::map< std::string, Type > m_types;

		static ScriptEngine * s_se;
	};
}