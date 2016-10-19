// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/scene/ISceneComponent.h>
#include <dxi/core/IOS.h>

namespace dxi
{
	namespace scene
	{
		class SceneComponent : public ISceneComponent
		{
		public:
			SceneComponent( core::IOS * os, std::string name );
			~SceneComponent();

			core::IOS * GetOS();
			const core::IOS * GetOS() const;

			const char * GetName() const override;

			bool IsEnabled() const override;
			void SetEnabled( bool enabled ) override;
			
			void OnAttach( Scene * scene ) override {} 
			void OnDetach( Scene * scene ) override {}
			void OnInit( Scene * scene ) override {}
			void OnStart( Scene * scene ) override {}
			void OnUpdate( Scene * scene, const RenderInfo & renderInfo ) override {}
			void OnRender( Scene * scene, const RenderInfo & renderInfo ) override {}
			void OnSuspend() override {}
			void OnResume() override {}

		private:
			core::IOS * m_os;
			std::string m_name;
			bool m_enabled;
		};
	}
}