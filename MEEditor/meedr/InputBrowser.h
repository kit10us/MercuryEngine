// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/IGame.h>
#include <meedr/ui/Window.h>
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

		ui::Result * OnCreate( ui::Params params ) override;

		void Timer_UpdateInputData();

		ui::Result* OnAfterCreate( ui::Params params ) override;
		ui::Result * OnDestroy( ui::Params params ) override;
		ui::Result* OnControlCommand( ui::ControlMessage message ) override;
		ui::Result* OnUserMessage( ui::UserMessageData message ) override;
	};
}
