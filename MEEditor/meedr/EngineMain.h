// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/IGame.h>
#include <meedr/ui/Window.h>

#define RESOURCEBROWSER_CLOSED	0
#define INPUTBROWSER_CLOSED		1
#define SCRIPTEDITOR_CLOSED		2
#define SCENEVIEWER_CLOSED		3
 								
namespace meedr
{
	class EngineMain : public ui::Window
	{
		me::IGame * m_game;
		int m_openChildren;
		IWindow::ptr m_resourceBrowser;
		IWindow::ptr m_inputBrowser;
		IWindow::ptr m_sceneViewer;
		IWindow::ptr m_scriptEditor;
	public:
		EngineMain( me::IGame * game );

		ui::IResult* OnControlCommand( ui::ControlMessage message ) override;
		ui::IResult* OnUserMessage( ui::UserMessageData message ) override;
	};
}