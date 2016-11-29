// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/scene/ISceneComponent.h>
#include <me/scene2d/IElement.h>
#include <me/scene2d/Layer.h>
#include <me/IGame.h>

namespace me
{
	namespace scene2d
	{
		class CanvasComponent : public scene::ISceneComponent
		{
		public:
			typedef std::shared_ptr< scene2d::CanvasComponent > ptr;

			CanvasComponent( IGame * game );	  

			const char * GetName() const override;

			bool IsEnabled() const override;
			void SetEnabled( bool enabled ) override;

			void OnAttach( scene::Scene * scene ) override;

			void OnDetach( scene::Scene * scene ) override;

			void OnInit( scene::Scene * scene ) override;

			void OnStart( scene::Scene * scene ) override;
			
			void OnUpdate( scene::Scene * scene, IRenderer * renderer, const RenderInfo & renderInfo ) override;

			void OnRender( scene::Scene * scene, IRenderer * renderer, const RenderInfo & renderInfo ) override;

			void OnSuspend() override;

			void OnResume() override;

			Layer * GetLayer();

		private:
			IGame * m_game;
			bool m_enabled;
			unify::V2< float > m_position;
			unify::Size< float > m_size;
			unify::Matrix m_projection;

			Layer::ptr m_layer;
		};
	}
}