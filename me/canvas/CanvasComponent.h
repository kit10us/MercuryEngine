// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/scene/component/SceneComponent.h>
#include <me/canvas/IElement.h>
#include <me/canvas/Layer.h>
#include <me/game/IGame.h>

namespace me::canvas
{
	/// <sumamry>
	/// Provides Mercury Canvas support to scenes via a scene component.
	/// It contains a Canvas layer, which is the root most node for canvises.
	/// </summary>
	class CanvasComponent : public scene::component::SceneComponent
	{
		game::IGame* m_game;
		unify::V2< float > m_position;
		unify::Size< float > m_size;
		unify::Matrix m_projection;
		Layer::ptr m_layer;

	public:
		typedef std::shared_ptr< CanvasComponent > ptr;

		static char* Name();

		CanvasComponent( game::IGame * gameInstance );	  

		Layer * GetLayer();

	public: // ISceneComponent...
		void OnAttach( scene::IScene * scene ) override;
		void OnDetach( scene::IScene * scene ) override;
		void OnUpdate( const UpdateParams & params ) override;
		void OnRender( scene::RenderGirl & renderGirl ) override;
		void OnSuspend() override;											
		void OnResume() override;

	public: //IComponent...
		std::string GetWhat() const override;
	};
}