// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/IGame.h>
#include <ui/Window.h>

namespace meedr
{
	class ScriptEditor : public ui::Window
	{
	private:
		me::IGame * m_game;
		class SceneViewer* m_sceneViewer;

	public:
		ScriptEditor( SceneViewer* parent, int nCmdShow, int x, int y, me::IGame * game );

		void LoadFile( unify::Path path );

		ui::IResult* OnCreate( ui::message::Params params ) override; 
		ui::IResult* OnAfterCreate( ui::message::Params params ) override;
		ui::IResult* OnDestroy( ui::message::Params params ) override;
		ui::IResult* OnControlCommand( ui::message::ControlCommand message ) override;
	};
}
