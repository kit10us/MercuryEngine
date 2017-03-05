// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/IGame.h>
#include <meedr/ui/Window.h>

namespace meedr
{
	class ScriptEditor : public meedr::ui::Window
	{
	private:
		me::IGame * m_game;

	public:
		ScriptEditor( IWindow* parent, int nCmdShow, int x, int y, me::IGame * game );

		void LoadFile( unify::Path path );

		ui::IResult* OnCreate( ui::Params params ) override; 
		ui::IResult* OnDestroy( ui::Params params ) override;
		ui::IResult* OnControlCommand( ui::ControlMessage message ) override;
	};
}
