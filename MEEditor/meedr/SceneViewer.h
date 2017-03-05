// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/IGame.h>
#include <me/scene/SceneManager.h>
#include <meedr/ui/Window.h>

namespace meedr
{
	class SceneViewer : public meedr::ui::Window
	{
	public:
		SceneViewer::SceneViewer( IWindow* parent, int nCmdShow, int x, int y, me::IGame * game );

		void UpdateSceneList();
		void UpdateObjectList();
		void UpdateObject();
		void OpenObjectComponent();

		ui::IResult* OnAfterCreate( ui::Params ) override;
		ui::IResult* OnDestroy( ui::Params params ) override;
		ui::IResult* OnControlCommand( ui::ControlMessage message ) override;

	private:
		me::IGame * m_game;
		me::scene::SceneManager * m_sceneManager;
	};
}