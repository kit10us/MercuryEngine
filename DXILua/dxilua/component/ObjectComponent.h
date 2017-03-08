// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/ScriptEngine.h>
#include <me/scene/IObjectComponent.h>
#include <me/scene/Object.h>
#include <me/IGame.h>

namespace melua
{
	namespace component
	{
		class ObjectComponent : public me::scene::IObjectComponent
		{
		protected:
			ObjectComponent( ObjectComponent & component );

		public:
			ObjectComponent( lua_State * state, me::IGame * game, std::string myName, unify::Path path );
			~ObjectComponent();

			std::string GetType() const;
			std::string GetWhat() const;

			bool IsEnabled() const override;
			void SetEnabled( bool enabled ) override;

			bool Updateable() const { return false; }
			bool Renderable() const { return false; }

			void OnAttach( me::scene::Object * object ) override;
			void OnDetach() override;
			void OnInit() override;
			void OnStart() override;
			void OnUpdate( me::UpdateParams params ) override;
			void CollectGeometry( me::GeometryCache & cache, const unify::FrameLite * frame ) override;
			void OnSuspend() override;
			void OnResume() override;

			me::scene::IObjectComponent * Duplicate();

			int GetValueCount() const override;
			bool ValueExists( std::string name ) const override;
			std::string GetValueName( int index ) const override;
			int FindValueIndex( std::string name ) const override;
			std::string GetValue( int index ) const override;
			std::string GetValue( std::string name ) const override;
			bool SetValue( int index, std::string value ) override;
			bool SetValue( std::string name, std::string value ) override;

		private:
			void CallMember( std::string function );

			lua_State * m_state;
			me::IGame * m_game;
			me::scene::Object * m_object;
			std::string m_name;
			unify::Path m_path;
			bool m_enabled;
		};
	}
}
