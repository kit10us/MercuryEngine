// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <DXIPhysX.h>
#include <dxiphysx/GameComponent.h>
#include <me/IObjectComponent.h>
#include <me/IOS.h>
#include <PxPhysicsAPI.h>
#include <dxiphysx/Releaser.h>

namespace dxiphysx
{
	namespace objectcomponent
	{
		class RigidBody : public me::IObjectComponent
		{
		protected:
			RigidBody( RigidBody & rigidBody );

		public:
			DXIPHYSX_API RigidBody( me::IOS * os, GameComponent * gameComponent );
			~RigidBody();

			me::IOS * GetOS();
			const me::IOS * GetOS() const;

			std::string GetName() const override;

			bool IsEnabled() const override;
			void SetEnabled( bool enabled ) override;
									   
			void OnAttach( me::Object * object ) override;
			void OnDetach( me::Object * object ) override {}
			void OnInit( me::Object * object ) override {}
			void OnStart( me::Object * object ) override {}
			void OnUpdate( me::Object * object, const me::RenderInfo & renderInfo ) override {}
			void OnRender( me::Object * object, const me::RenderInfo & renderInfo ) override {}
			void OnSuspend( me::Object * object ) override {}
			void OnResume( me::Object * object ) override {}

			me::IObjectComponent * Duplicate() override;

			DXIPHYSX_API physx::PxRigidBody * GetRigidBody();
			const physx::PxRigidBody * GetRigidBody() const;

		private:
			me::IOS * m_os;
			bool m_enabled;
			GameComponent * m_gameComponent;
			std::shared_ptr< physx::PxRigidBody > m_rigidBody;
		};
	}
}