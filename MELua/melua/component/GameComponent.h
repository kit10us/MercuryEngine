// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <melua/ScriptEngine.h>
#include <melua/Script.h>
#include <me/game/GameComponent.h>

namespace melua
{
	namespace component
	{
		class GameComponent : public me::game::GameComponent
		{
		public:
			static char* Name();

			GameComponent( Script * script );
			~GameComponent();

		public: // IGameComponent...
			void OnAttach( me::game::IGame * gameInstance ) override;
			void OnBeforeStartup() override;
			void OnAfterStartup() override;
			void OnUpdate( const me::UpdateParams & params ) override;
			void OnRender( const me::render::Params & params ) override;
			void OnDetach( me::game::IGame * gameInstance ) override;

		public: // IComponent...
			std::string GetWhat() const override;

			int GetValueCount() const override;
			bool ValueExists( std::string ) const override;
			std::string GetValueName( int index ) const override;
			int FindValueIndex( std::string name ) const override;
			std::string GetValue( int index ) const override;
			bool SetValue( int index, std::string value ) override;

		private:
			void CallMember( std::string function );

			Script * m_script;
		};
	}
}
