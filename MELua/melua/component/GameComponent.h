// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <melua/ScriptEngine.h>
#include <me/GameComponent.h>

namespace melua
{
	namespace component
	{
		class GameComponent : public me::GameComponent
		{
		public:
			GameComponent( lua_State * state, std::string luaName, unify::Path path );
			~GameComponent();

		public: // IGameComponent...
			void OnAttach( me::IGame * game ) override;
			void OnBeforeStartup() override;
			void OnAfterStartup() override;
			void OnUpdate( me::UpdateParams params ) override;
			void OnRender( me::RenderParams params ) override;
			void OnDetach( me::IGame * game ) override;

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