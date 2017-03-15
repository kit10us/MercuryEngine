// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/scene/SceneComponent.h>
#include <me/canvas/IElement.h>
#include <me/canvas/Layer.h>
#include <me/IGame.h>

namespace me
{
	namespace canvas
	{
		class CanvasComponent : public scene::SceneComponent
		{
		public:
			typedef std::shared_ptr< canvas::CanvasComponent > ptr;

			CanvasComponent( IGame * game );	  

			Layer * GetLayer();

		public: // ISceneComponent...
			void OnAttach( scene::Scene * scene ) override;
			void OnDetach( scene::Scene * scene ) override;
			void OnInit( scene::Scene * scene ) override;  
			void OnStart( scene::Scene * scene ) override; 			
			void OnUpdate( scene::Scene * scene, UpdateParams params ) override;
			void OnRender( scene::Scene * scene, scene::RenderGirl & renderGirl ) override;
			void OnSuspend() override;											
			void OnResume() override;

		public: //IComponent...
			std::string GetWhat() const override;

		private:
			IGame * m_game;
			unify::V2< float > m_position;
			unify::Size< float > m_size;
			unify::Matrix m_projection;
			Layer::ptr m_layer;
		};
	}
}