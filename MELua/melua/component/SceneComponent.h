// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <melua/ScriptEngine.h>
#include <me/scene/component/SceneComponent.h>
#include <melua/Script.h>

namespace melua
{
	namespace component
	{
		class SceneComponent : public me::scene::component::SceneComponent
		{
		public:
			SceneComponent( me::game::IGame * gameInstance, Script * script );
			~SceneComponent();

			void CallMember( std::string function );

			const Script * GetScript() const;

		public: // ISceneComponent...
			void OnAttach( me::scene::IScene * scene ) override;
			void OnDetach( me::scene::IScene * scene ) override;
			void OnBeforeStart() override;
			void OnAfterStart() override;
			void OnUpdate( const me::UpdateParams & params ) override;
			void OnSuspend() override;
			void OnResume() override;
			void OnEnd() override;

		public: // IComponent...
			std::string GetWhat() const override;

		private:
			Script * m_script;
			me::game::IGame* m_game;
		};
	}
}
