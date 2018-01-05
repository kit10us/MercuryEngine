// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <MEPhysics.h>
#include <me/object/component/ObjectComponent.h>
#include <me/object/Object.h>
#include <me/game/IGame.h>
#include <me/object/action/IObjectAction.h>

#pragma once 

namespace mephy
{
	namespace phy3d
	{
		namespace collider
		{
			class ColliderBase : public me::object::component::ObjectComponent
			{
			protected:
				ColliderBase( ColliderBase & component );

				ColliderBase( std::string name, bool moveable, float mass );

			public:
				~ColliderBase();

				MEPHYSICS_API void SetOnEnterAction( me::object::action::IObjectAction::ptr action );
				MEPHYSICS_API void SetOnExitAction( me::object::action::IObjectAction::ptr action );

				virtual void TestCollision( ColliderBase* entity, const me::UpdateParams & params ) = 0;

				void SetMoveable( bool moveable );
				bool GetMoveable() const;

				void SetMass( float mass );
				float GetMass() const;

				void CaptureEarly();
				const unify::Matrix & GetEarly() const;

			protected:
				void PerformOnEnter( ColliderBase * collider, const me::UpdateParams & params );
				void PerformOnExit( ColliderBase * collider, const me::UpdateParams & params );

			public: // IGameComponent...
				void OnAttach( me::object::Object * object ) override;
				void OnDetach( me::object::Object * objecct ) override;

			public: // IComponent...

			private:
				me::object::action::IObjectAction::ptr m_onEnter;
				me::object::action::IObjectAction::ptr m_onExit;
				bool m_isSolid;
				bool m_moveable;
				float m_mass;
				unify::Matrix m_early;
			};
		}
	}
}
