// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/scene/IObjectComponent.h>
#include <dxi/core/IOS.h>
#include <PxPhysicsAPI.h>
#include <dxiphysx/Releaser.h>

namespace dxiphysx
{
	namespace objectcomponent
	{
		class RigidBody : public dxi::scene::IObjectComponent
		{
		public:
			RigidBody( dxi::core::IOS * os, physx::PxPhysics * physics );
			~RigidBody();

			dxi::core::IOS * GetOS();
			const dxi::core::IOS * GetOS() const;

			std::string GetName() const override;

			bool IsEnabled() const override;
			void SetEnabled( bool enabled ) override;
									   
			void OnAttach( dxi::scene::Object * object ) override;
			void OnDetach( dxi::scene::Object * object ) override {}
			void OnInit( dxi::scene::Object * object ) override {}
			void OnStart( dxi::scene::Object * object ) override {}
			void OnUpdate( const dxi::RenderInfo & renderInfo ) override {}
			void OnRender( const dxi::RenderInfo & renderInfo ) override {}
			void OnSuspend() override {}
			void OnResume() override {}

			physx::PxRigidBody * GetRigidBody();
			const physx::PxRigidBody * GetRigidBody() const;

		private:
			dxi::core::IOS * m_os;
			bool m_enabled;
			physx::PxPhysics * m_physics;
			std::shared_ptr< physx::PxRigidBody > m_rigidBody;
		};
	}
}