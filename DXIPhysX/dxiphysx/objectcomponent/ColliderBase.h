// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <DXIPhysX.h>
#include <me/scene/IObjectComponent.h>
#include <dxiphysx/GameComponent.h>
#include <me/IOS.h>
#include <PxPhysicsAPI.h>
#include <dxiphysx/Releaser.h>

namespace dxiphysx
{
	namespace objectcomponent
	{
		class ColliderBase : public me::scene::IObjectComponent
		{
		protected:
			ColliderBase( ColliderBase & colliderBase );

		public:
			ColliderBase( me::IOS * os, GameComponent * gameComponent );
			~ColliderBase();

			me::IOS * GetOS();
			const me::IOS * GetOS() const;

			bool IsEnabled() const override;
			void SetEnabled( bool enabled ) override;

			bool Updateable() const { return false; }
			bool Renderable() const { return false; }
									   
			void OnAttach( me::scene::Object * object ) override;
			void OnDetach() override {}
			void OnInit() override {}
			void OnStart() override {}
			void OnUpdate( me::IRenderer * renderer, const me::RenderInfo & renderInfo ) override {}
			void CollectGeometry( me::GeometryCache & cache, const unify::FrameLite * frame ) override {}
			void OnSuspend() override {}
			void OnResume() override {}

			DXIPHYSX_API physx::PxShape * GetShape();
			DXIPHYSX_API const physx::PxShape * GetShape() const;

		private:
			me::IOS * m_os;
			bool m_enabled;

		protected:
			GameComponent * m_gameComponent;
			std::shared_ptr< physx::PxShape > m_shape;
			std::shared_ptr< physx::PxMaterial > m_material;
		};
	}
}