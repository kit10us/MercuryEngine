// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <DXIPhysX.h>
#include <dxiphysx/GameComponent.h>
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
		protected:
			RigidBody( RigidBody & rigidBody );

		public:
			DXIPHYSX_API RigidBody( dxi::core::IOS * os, GameComponent * gameComponent );
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
			void OnUpdate( dxi::scene::Object * object, const dxi::RenderInfo & renderInfo ) override {}
			void OnRender( dxi::scene::Object * object, const dxi::RenderInfo & renderInfo ) override {}
			void OnSuspend( dxi::scene::Object * object ) override {}
			void OnResume( dxi::scene::Object * object ) override {}

			dxi::scene::IObjectComponent * Duplicate() override;

			DXIPHYSX_API physx::PxRigidBody * GetRigidBody();
			const physx::PxRigidBody * GetRigidBody() const;

		private:
			dxi::core::IOS * m_os;
			bool m_enabled;
			GameComponent * m_gameComponent;
			std::shared_ptr< physx::PxRigidBody > m_rigidBody;
		};
	}
}