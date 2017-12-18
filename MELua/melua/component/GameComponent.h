// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <melua/ScriptEngine.h>
#include <me/game/GameComponent.h>

namespace melua
{
	namespace component
	{
		class GameComponent : public me::game::GameComponent
		{
		public:
			static char* Name();

			GameComponent( lua_State * state, std::string luaName, unify::Path path );
			~GameComponent();

		public: // IGameComponent...
			void OnAttach( me::game::IGame * gameInstance ) override;
			void OnBeforeStartup() override;
			void OnAfterStartup() override;
			void OnUpdate( const me::UpdateParams & params ) override;
			void OnRender( me::render::Params params ) override;
			void OnDetach( me::game::IGame * gameInstance ) override;

		public: // IComponent...
			std::string GetWhat() const override;

		private:
			void CallMember( std::string function );

			lua_State * m_state;
			std::string m_luaName;
			unify::Path m_path;
		};
	}
}
