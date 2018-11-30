// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <melua/ScriptEngine.h>
#include <me/scene/SceneComponent.h>
#include <melua/Script.h>

namespace melua
{
	namespace component
	{
		class SceneComponent : public me::scene::SceneComponent
		{
		public:
			SceneComponent( me::game::IGame * gameInstance, Script * script );
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
			Script * m_script;
			me::game::IGame* m_game;
		};
	}
}
