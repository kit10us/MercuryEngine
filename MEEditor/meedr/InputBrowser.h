// Copyright (c) 2002 - 2018, Quentin S. Smith
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
		class SceneViewer* m_sceneViewer;
		bool m_closing;
		std::thread m_updateData;
	public:
		InputBrowser( SceneViewer* parent, int nCmdShow, int x, int y, me::IGame * game );
		~InputBrowser();

		void UpdateInputData();
		void UpdateInputManagerList();
		void UpdateInputDeviceInputList();
		void Timer_UpdateInputData();

		ui::IResult* OnAfterCreate( ui::message::Params params ) override;
		ui::IResult* OnDestroy( ui::message::Params params ) override;
		ui::IResult* OnControlCommand( ui::message::ControlCommand message ) override;
		ui::IResult* OnUser( ui::message::User message ) override;
	};
}
