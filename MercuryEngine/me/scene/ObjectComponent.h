// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/scene/IObjectComponent.h>
#include <me/IOS.h>

namespace me
{
	namespace scene
	{
		class ObjectComponent : public IObjectComponent
		{
		protected:
			ObjectComponent( ObjectComponent & component );

		public:
			ObjectComponent( IOS * os );
			~ObjectComponent();

			IOS * GetOS();
			const IOS * GetOS() const;

			bool IsEnabled() const override;
			void SetEnabled( bool enabled ) override;

			bool Updateable() const override { return false; }
			bool Renderable() const override { return false; }			   
										   
			void OnAttach( Object * object ) override {}
			void OnDetach() override {}
			void OnInit() override {}
			void OnStart() override {}
			void OnUpdate( IRenderer * renderer, const RenderInfo & renderInfo ) override {}
			void CollectGeometry( GeometryCache & cache, const unify::FrameLite * frame ) override {}
			void OnSuspend() override {}
			void OnResume() override {}

		private:
			IOS * m_os;
			bool m_enabled;
		};
	}
}