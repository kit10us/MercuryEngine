// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <MEPhysX.h>
#include <mephysx/GameComponent.h>
#include <me/scene/ObjectComponent.h>
#include <me/IOS.h>
#include <PxPhysicsAPI.h>
#include <mephysx/Releaser.h>

namespace mephysx
{
	namespace objectcomponent
	{
		class RigidStatic : public me::scene::ObjectComponent
		{
		protected:
			RigidStatic( RigidStatic & rigidStatic );

		public:
			MEPHYSX_API RigidStatic( GameComponent * gameComponent );
			~RigidStatic();

			std::string GetWhat() const override;

			void OnAttach( me::scene::Object * object ) override;
			void OnDetach( me::scene::Object * object ) override;

			me::scene::IObjectComponent * Duplicate() override;

			MEPHYSX_API physx::PxRigidStatic * GetRigidStatic();
			const physx::PxRigidStatic * GetRigidStatic() const;

		private:
			GameComponent * m_gameComponent;
			std::shared_ptr< physx::PxRigidStatic > m_rigidStatic;
		};
	}
}