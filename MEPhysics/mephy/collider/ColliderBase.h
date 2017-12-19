// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <MEPhysics.h>
#include <me/object/component/ObjectComponent.h>
#include <me/object/Object.h>
#include <me/game/IGame.h>
#include <me/object/action/IObjectAction.h>
#include <mephy/Entity.h>

#pragma once 

namespace mephy
{
	namespace collider
	{
		class ColliderBase : public me::object::component::ObjectComponent
		{
		protected:
			ColliderBase( ColliderBase & component );

			ColliderBase( std::string name );

		public:
			~ColliderBase();

			MEPHYSICS_API void SetOnEnterAction( me::object::action::IObjectAction::ptr action );
			MEPHYSICS_API void SetOnExitAction( me::object::action::IObjectAction::ptr action );

			virtual void TestCollision( Entity* entity, const me::UpdateParams & params ) = 0;

		protected:
			void PerformOnEnter( Entity * entity, const me::UpdateParams & params );
			void PerformOnExit( Entity * entity, const me::UpdateParams & params );

		public: // IGameComponent...

		public: // IComponent...

		private:
			me::object::action::IObjectAction::ptr m_onEnter;
			me::object::action::IObjectAction::ptr m_onExit;
			bool m_isSolid;
		};
	}
}
