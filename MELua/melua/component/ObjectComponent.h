// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <melua/ScriptEngine.h>
#include <me/object/ObjectComponent.h>
#include <me/object/Object.h>
#include <me/IGame.h>

namespace melua
{
	namespace component
	{
		class ObjectComponent : public me::object::ObjectComponent
		{
		protected:
			ObjectComponent( ObjectComponent & component );

		public:
			ObjectComponent( me::IGame * game, lua_State * state, std::string myName, unify::Path path );
			~ObjectComponent();

			std::string GetLuaName() const;

		public: // IGameComponent...
			void OnStart() override;
			void OnUpdate( me::UpdateParams params ) override;
			void OnSuspend() override;
			void OnResume() override;

			me::object::IObjectComponent::ptr Duplicate();

		public: // IComponent...
			std::string GetWhat() const;
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
			std::string m_luaName;
			unify::Path m_path;
		};
	}
}
