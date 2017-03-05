// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/IGame.h>
#include <meedr/ui/Window.h>

#define RESOURCEBROWSER_CLOSED	0
#define INPUTBROWSER_CLOSED		1
#define SCRIPTEDITOR_CLOSED		2
#define SCENEVIEWER_CLOSED		3
#define SCRIPTEDITOR_OPEN		4
#define LOGVIEWER_CLOSED		5
 								
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
		IWindow::ptr m_logViewer;
	public:
		EngineMain( me::IGame * game );
		
		me::IGame * GetGame() const;

		void OpenResourceBrowser();
		void OpenInputBrowser();
		void OpenScriptEditor( unify::Path source );
		void OpenSceneViewer();
		void OpenLogViewer();

		ui::IResult* OnControlCommand( ui::ControlMessage message ) override;
		ui::IResult* OnUserMessage( ui::UserMessageData message ) override;
	};
}