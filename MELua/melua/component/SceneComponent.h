// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <melua/ScriptEngine.h>
#include <me/scene/SceneComponent.h>

namespace melua
{
	namespace component
	{
		class SceneComponent : public me::scene::SceneComponent
		{
		public:
			SceneComponent( me::IGame * game, lua_State * state, std::string luaName, unify::Path path );
			~SceneComponent();

			void CallMember( std::string function );

		public: // ISceneComponent...
			void OnAttach( me::scene::IScene * scene ) override;
			void OnDetach( me::scene::IScene * scene ) override;
			void OnBeforeStart() override;
			void OnAfterStart() override;
			void OnUpdate( me::UpdateParams params ) override;
			void OnSuspend() override;
			void OnResume() override;
			void OnEnd() override;

		public: // IComponent...
			std::string GetWhat() const override;

		private:
			std::string m_luaName;
			unify::Path m_path;
			lua_State * m_state;
			me::IGame* m_game;
		};
	}
}
