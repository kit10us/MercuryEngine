// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/IGame.h>
#include <meedr/ui/Window.h>

namespace meedr
{
	class LogViewer : public meedr::ui::Window, me::ILogListener
	{
	private:
		me::IGame * m_game;

	public:
		LogViewer( IWindow* parent, int nCmdShow, int x, int y, me::IGame * game );
		~LogViewer();

		// me::ILogListener...
		void Log( std::string text ) override;

		ui::IResult* OnCreate( ui::Params params ) override; 
		ui::IResult* OnAfterCreate( ui::Params params ) override;
		ui::IResult* OnDestroy( ui::Params params ) override;
		ui::IResult* OnControlCommand( ui::ControlMessage message ) override;
	};
}
