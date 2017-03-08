// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/IGame.h>
#include <ui/Window.h>
#include <thread>

namespace meedr
{
	class InputBrowser : public ui::Window
	{
		me::IGame * m_game;
		bool m_closing;
		std::thread m_updateData;
	public:
		InputBrowser( IWindow* parent, int nCmdShow, int x, int y, me::IGame * game );
		~InputBrowser();

		void UpdateInputData();
		void UpdateInputManagerList();
		void UpdateInputSourceInputList();
		void Timer_UpdateInputData();

		ui::IResult* OnAfterCreate( ui::Params params ) override;
		ui::IResult* OnDestroy( ui::Params params ) override;
		ui::IResult* OnControlCommand( ui::ControlMessage message ) override;
		ui::IResult* OnUserMessage( ui::UserMessageData message ) override;
	};
}
