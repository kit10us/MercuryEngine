// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <MEPhysX.h>
#include <mephysx/GameComponent.h>
#include <me/object/ObjectComponent.h>
#include <PxPhysicsAPI.h>
#include <mephysx/Releaser.h>

namespace mephysx
{
	namespace objectcomponent
	{
		class RigidBody : public me::object::ObjectComponent
		{
		protected:
			RigidBody( RigidBody & rigidBody );

		public:
			MEPHYSX_API RigidBody( GameComponent * gameComponent );
			~RigidBody();

			std::string GetWhat() const override;

			void OnAttach( me::object::Object * object ) override;
			void OnDetach( me::object::Object * object ) override;

			me::object::IObjectComponent::ptr Duplicate() override;

			MEPHYSX_API physx::PxRigidBody * GetRigidBody();
			const physx::PxRigidBody * GetRigidBody() const;

		private:
			GameComponent * m_gameComponent;
			std::shared_ptr< physx::PxRigidBody > m_rigidBody;
		};
	}
}