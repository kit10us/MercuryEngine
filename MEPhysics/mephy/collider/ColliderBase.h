// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <MEPhysics.h>
#include <me/object/ObjectComponent.h>
#include <me/object/Object.h>
#include <me/IGame.h>
#include <me/action/ActionList.h>

#pragma once 

namespace mephy
{
	namespace collider
	{
		class ColliderBase : public me::object::ObjectComponent
		{
		protected:
			ColliderBase( ColliderBase & component );

			ColliderBase( std::string name );

		public:
			~ColliderBase();

			void AddOnEnterAction( me::action::IAction::ptr onEnterAction );
			void AddOnExitAction( me::action::IAction::ptr onEnterAction );

		public: // IGameComponent...
			void OnStart() override;
			void OnUpdate( me::UpdateParams params ) override;
			void CollectGeometry( me::GeometryCache & cache, const unify::FrameLite * frame ) override;
			void OnSuspend() override;
			void OnResume() override;

		public: // IComponent...
			std::string GetWhat() const;

		private:
			me::action::ActionList m_onEnter;
			me::action::ActionList m_onExit;
		};
	}
}
