// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <melua/ScriptEngine.h>
#include <melua/Script.h>
#include <me/game/component/GameComponent.h>

namespace melua
{
	namespace component
	{
		class GameComponent : public me::game::component::GameComponent
		{
		public:
			static char* Name();

			GameComponent( Script * script );
			~GameComponent();

			const Script * GetScript() const;

		public: // IGameComponent...
			void OnAttach( me::game::IGame * gameInstance ) override;
			void OnBeforeStartup() override;
			void OnAfterStartup() override;
			void OnUpdate( const me::UpdateParams & params ) override;
			void OnRender( const me::render::Params & params ) override;
			void OnDetach( me::game::IGame * gameInstance ) override;

		public: // IComponent...
			std::string GetWhat() const override;

		private:
			void CallMember( std::string function );

			Script * m_script;
		};
	}
}
