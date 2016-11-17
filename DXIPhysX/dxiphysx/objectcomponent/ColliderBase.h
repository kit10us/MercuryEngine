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

			bool Renderable() const { return false; }
									   
			void OnAttach( me::scene::Object * object ) override;
			void OnDetach( me::scene::Object * object ) override {}
			void OnInit( me::scene::Object * object ) override {}
			void OnStart( me::scene::Object * object ) override {}
			void OnUpdate( me::scene::Object * object, const me::IRenderer * renderer, const me::RenderInfo & renderInfo ) override {}
			void OnRender( me::scene::Object * object, const me::IRenderer * renderer, const me::RenderInfo & renderInfo, std::list< me::RenderSet > & list, unify::Matrix transform ) override {}
			void OnSuspend( me::scene::Object * object ) override {}
			void OnResume( me::scene::Object * object ) override {}

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