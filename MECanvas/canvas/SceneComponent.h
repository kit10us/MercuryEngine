// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <canvas/IElement.h>
#include <canvas/Layer.h>
#include <me/game/IGame.h>
#include <me/scene/SceneComponent.h>

namespace canvas
{
	class SceneComponent : public me::scene::SceneComponent
	{
	public:
		typedef std::shared_ptr< canvas::SceneComponent > ptr;

		static char* Name();

		SceneComponent( me::game::IGame * gameInstance );

		Layer * GetLayer();

	public: // ISceneComponent...
		void OnAttach( me::scene::IScene * scene ) override;
		void OnDetach( me::scene::IScene * scene ) override;
		void OnUpdate( const me::UpdateParams & params ) override;
		void OnRender( me::scene::RenderGirl & renderGirl ) override;
		void OnSuspend() override;											
		void OnResume() override;

	public: //IComponent...
		std::string GetWhat() const override;

	private:
		me::game::IGame * m_game;
		unify::V2< float > m_position;
		unify::Size< float > m_size;
		unify::Matrix m_projection;
		Layer::ptr m_layer;
	};
}