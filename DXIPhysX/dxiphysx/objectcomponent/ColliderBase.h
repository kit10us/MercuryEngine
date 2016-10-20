// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <DXIPhysX.h>
#include <dxi/scene/IObjectComponent.h>
#include <dxiphysx/GameComponent.h>
#include <dxi/core/IOS.h>
#include <PxPhysicsAPI.h>
#include <dxiphysx/Releaser.h>

namespace dxiphysx
{
	namespace objectcomponent
	{
		class ColliderBase : public dxi::scene::IObjectComponent
		{
		public:
			ColliderBase( dxi::core::IOS * os, GameComponent * gameComponent );
			~ColliderBase();

			dxi::core::IOS * GetOS();
			const dxi::core::IOS * GetOS() const;

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

			DXIPHYSX_API physx::PxShape * GetShape();
			DXIPHYSX_API const physx::PxShape * GetShape() const;

		private:
			dxi::core::IOS * m_os;
			bool m_enabled;

		protected:
			GameComponent * m_gameComponent;
			std::shared_ptr< physx::PxShape > m_shape;
			std::shared_ptr< physx::PxMaterial > m_material;
		};
	}
}