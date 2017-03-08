// All Rights Reserved

#include <dxilua/ScriptEngine.h>
#include <me/IGameComponent.h>
#include <me/scene/Object.h>
#include <me/IGame.h>

namespace melua
{
	namespace component
	{
		class GameComponent : public me::IGameComponent
		{
		public:
			GameComponent( lua_State * state, me::IGame * game, std::string name, unify::Path path );
			~GameComponent();

			std::string GetName() const override;

			void OnAttach( me::IGame * game ) override;
			void OnBeforeStartup( me::IGame * game ) override;
			void OnAfterStartup( me::IGame * game ) override;
			void OnUpdate( me::IGame * game, me::UpdateParams params ) override;
			void OnRender( me::IGame * game, me::RenderParams params ) override;
			void OnDetach( me::IGame * game ) override;

		private:
			void CallMember( std::string function );

			lua_State * m_state;
			me::IGame * m_game;
			std::string m_luaName;
			unify::Path m_path;
			bool m_enabled;
		};
	}
}
