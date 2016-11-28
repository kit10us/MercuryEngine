// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/scene/ISceneComponent.h>
#include <me/IOS.h>

namespace me
{
	namespace scene
	{
		class SceneComponent : public ISceneComponent
		{
		public:
			SceneComponent( IOS * os, std::string name );
			~SceneComponent();

			IOS * GetOS();
			const IOS * GetOS() const;

			const char * GetName() const override;

			bool IsEnabled() const override;
			void SetEnabled( bool enabled ) override;
			
			void OnAttach( Scene * scene ) override {}
			void OnDetach( Scene * scene ) override {}
			void OnInit( Scene * scene ) override {}
			void OnStart( Scene * scene ) override {}
			void OnUpdate( Scene * scene, IRenderer * renderer, const RenderInfo & renderInfo ) override {}
			void OnRender( Scene * scene, IRenderer * renderer, const RenderInfo & renderInfo ) override {}
			void OnSuspend() override {}
			void OnResume() override {}

		private:
			IOS * m_os;
			std::string m_name;
			bool m_enabled;
		};
	}
}