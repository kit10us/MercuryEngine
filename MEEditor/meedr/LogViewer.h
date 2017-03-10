// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/IGame.h>
#include <ui/Window.h>

namespace meedr
{
	class LogViewer : public ui::Window, me::ILogListener
	{
	private:
		me::IGame * m_game;

	public:
		LogViewer( IWindow* parent, int nCmdShow, int x, int y, me::IGame * game );
		~LogViewer();

		// me::ILogListener...
		void Log( std::string text ) override;

		ui::IResult* OnCreate( ui::message::Params params ) override; 
		ui::IResult* OnAfterCreate( ui::message::Params params ) override;
		ui::IResult* OnDestroy( ui::message::Params params ) override;
		ui::IResult* OnControlCommand( ui::message::ControlCommand message ) override;
	};
}
