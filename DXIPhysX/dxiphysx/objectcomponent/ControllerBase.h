// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <DXIPhysX.h>
#include <dxiphysx/SceneComponent.h>

namespace dxiphysx
{
	namespace objectcomponent
	{
		class ControllerBase : public me::scene::IObjectComponent
		{
		protected:
			ControllerBase( ControllerBase & controller );

		public:
			ControllerBase( me::IOS * os, SceneComponent * gameComponent );
			~ControllerBase();

			me::IOS * GetOS();
			const me::IOS * GetOS() const;

			bool IsEnabled() const override;
			void SetEnabled( bool enabled ) override;

			bool Updateable() const { return false; }
			bool Renderable() const { return false; }

			DXIPHYSX_API physx::PxController * GetController();
			DXIPHYSX_API const physx::PxController * GetController() const;

			DXIPHYSX_API SceneComponent * GetScene();
			DXIPHYSX_API const SceneComponent * GetScene() const;

			// Snubbed overrides...
			void OnDetach() override {}
			void OnInit() override {}
			void OnStart() override {}
			void OnUpdate( me::IRenderer * renderer, const me::RenderInfo & renderInfo ) override {}
			void CollectGeometry( me::GeometryCache & cache, const unify::FrameLite * frame ) override {}
			void OnSuspend() override {}
			void OnResume() override {}

		private:
			me::IOS * m_os;
			bool m_enabled;

		protected:
			SceneComponent * m_sceneComponent;
			std::shared_ptr< physx::PxController > m_controller;
		};
	}
}