// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <MELuaDLL.h>
#include <me/game/component/GameComponent.h>
#include <me/scene/Scene.h>
#include <melua/Type.h>
#include <melua/Util.h>
#include <lua.hpp>

namespace melua
{
	enum class ExecuteResult
	{
		Pass,
		Fail,
		Pending
	};

	class ScriptEngine : public me::game::component::GameComponent
	{
	public:
		ScriptEngine( me::game::IGame * game );
		~ScriptEngine();

		ExecuteResult ExecuteString( std::string line );
		ExecuteResult ExecuteFile( unify::Path path );

		me::game::component::IGameComponent::ptr LoadGameScript( unify::Path path );

		me::object::component::IObjectComponent::ptr LoadObjectScript( unify::Path path );

		me::scene::component::ISceneComponent::ptr LoadSceneScript( unify::Path path );

		MELUADLL_API lua_State * GetState();

		MELUADLL_API void AddLibrary( const char * group, const luaL_Reg * list, int count );

		MELUADLL_API void AddType( Type type );

		std::list< std::string > GetType( std::string name ) const;

		MELUADLL_API static ScriptEngine* GetInstance();
		MELUADLL_API me::game::Game * GetGame();

		/// <summary>
		/// Asserts that our argument count for a function (top) is correct.
		/// Returns true if we want to continue execution, else returns false.
		/// </summary>
		bool Assert( bool isTrue, std::string message );

		/// <summary>
		/// Asserts that our top is of a certain amount. If it isn't, then
		/// in addition to Assert, we print the stack.
		/// </summary>
		bool AssertTop( int top );

		void OnBeforeStartup() override;

		void LogTypes();

	public: // IComponent...
		
	private:
		me::game::IGame * m_game;
		lua_State * m_state;
		size_t m_gameScriptCount;
		size_t m_objectScriptCount;

		// Type name { Function Names }
		std::map< std::string, std::list< std::string > > m_types;

		static ScriptEngine * s_se;	
	};
}