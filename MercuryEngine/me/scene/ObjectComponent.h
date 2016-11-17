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

			bool Renderable() const override { return false; }			   
										   
			void OnAttach( Object * object ) override {}
			void OnDetach( Object * object ) override {}
			void OnInit( Object * object ) override {}
			void OnStart( Object * object ) override {}
			void OnUpdate( Object * object, const IRenderer * renderer, const RenderInfo & renderInfo ) override {}
			void OnRender( Object * object, const IRenderer * renderer, const RenderInfo & renderInfo, std::list< RenderSet > & list, unify::Matrix transform ) override {}
			void OnSuspend( Object * object ) override {}
			void OnResume( Object * object ) override {}

		private:
			IOS * m_os;
			bool m_enabled;
		};
	}
}