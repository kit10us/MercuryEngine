// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <melua/ScriptEngine.h>
#include <me/object/component/ObjectComponent.h>
#include <me/object/Object.h>
#include <me/game/IGame.h>

namespace melua
{
	namespace component
	{
		class ObjectComponent : public me::object::component::ObjectComponent
		{
		protected:
			ObjectComponent( ObjectComponent & component );

		public:
			ObjectComponent( me::game::IGame * gameInstance, lua_State * state, std::string myName, unify::Path path );
			~ObjectComponent();

			std::string GetLuaName() const;

		public: // IGameComponent...
			void OnStart() override;
			void OnUpdate( const me::UpdateParams & params ) override;
			void OnSuspend() override;
			void OnResume() override;

			me::object::component::IObjectComponent::ptr Duplicate();

		public: // IComponent...
			std::string GetWhat() const;
			int GetValueCount() const override;
			bool ValueExists( std::string name ) const override;
			std::string GetValueName( int index ) const override;
			int FindValueIndex( std::string name ) const override;
			std::string GetValue( int index ) const override;
			bool SetValue( int index, std::string value ) override;

		private:
			void CallMember( std::string function );

			lua_State * m_state;
			me::game::IGame * m_game;
			std::string m_luaName;
			unify::Path m_path;
		};
	}
}
