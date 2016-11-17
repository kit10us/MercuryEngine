// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <DXIPhysX.h>
#include <dxiphysx/GameComponent.h>
#include <me/scene/IObjectComponent.h>
#include <me/IOS.h>
#include <PxPhysicsAPI.h>
#include <dxiphysx/Releaser.h>

namespace dxiphysx
{
	namespace objectcomponent
	{
		class RigidStatic : public me::scene::IObjectComponent
		{
		protected:
			RigidStatic( RigidStatic & rigidStatic );

		public:
			DXIPHYSX_API RigidStatic( me::IOS * os, GameComponent * gameComponent );
			~RigidStatic();

			me::IOS * GetOS();
			const me::IOS * GetOS() const;

			std::string GetName() const override;

			bool IsEnabled() const override;
			void SetEnabled( bool enabled ) override;

			bool Renderable() const { return false; }
									   
			void OnAttach( me::scene::Object * object ) override;
			void OnDetach( me::scene::Object * object ) override {}
			void OnInit( me::scene::Object * object ) override {}
			void OnStart( me::scene::Object * object ) override {}
			void OnUpdate( me::scene::Object * object, const me::IRenderer * renderer, const me::RenderInfo & renderInfo ) override {}
			void OnRender( me::scene::Object * object, const me::IRenderer * renderer, const me::RenderInfo & renderInfo, std::list< me::RenderSet > & list, unify::Matrix transform ) override {}
			void OnSuspend( me::scene::Object * object ) override {}
			void OnResume( me::scene::Object * object ) override {}

			me::scene::IObjectComponent * Duplicate() override;

			DXIPHYSX_API physx::PxRigidStatic * GetRigidStatic();
			const physx::PxRigidStatic * GetRigidStatic() const;

		private:
			me::IOS * m_os;
			bool m_enabled;
			GameComponent * m_gameComponent;
			std::shared_ptr< physx::PxRigidStatic > m_rigidStatic;
		};
	}
}