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
			SceneComponent( me::game::IGame * gameInstance, lua_State * state, std::string luaName, unify::Path path );
			~SceneComponent();

			void CallMember( std::string function );

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

			int GetValueCount() const override;
			bool ValueExists( std::string ) const override;
			std::string GetValueName( int index ) const override;
			int FindValueIndex( std::string name ) const override;
			std::string GetValue( int index ) const override;
			bool SetValue( int index, std::string value ) override;

		private:
			std::string m_luaName;
			unify::Path m_path;
			lua_State * m_state;
			me::game::IGame* m_game;
		};
	}
}
