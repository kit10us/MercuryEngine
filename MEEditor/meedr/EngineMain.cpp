// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/EngineMain.h>
#include <ui/Window.h>
#include <meedr/ResourceBrowser.h>
#include <meedr/InputBrowser.h>
#include <meedr/ScriptEditor.h>
#include <meedr/SceneViewer.h>
#include <meedr/LogViewer.h>

using namespace meedr;

EngineMain::EngineMain( me::IGame * game )
	: Window( (HWND)game->GetOSParameters().hWnd, L"EngineMainClass" )
	, m_game{ game }
	, m_openChildren{ 0 }
{
	int nCmdShow = game->GetOSParameters().nCmdShow;
	RECT rect{};
	::GetWindowRect( GetParentHandle(), &rect );
	int x = rect.right;
	int y = rect.top;

	using namespace create;
	AddContainer( new container::StackPanel( container::Stack::Vertical, 240, SizeToContentHeight() ) );
	AddControl( new Button( L"Resource Browser", FillWidth(), DefaultHeight() ), "ResourceBrowser" );
	AddControl( new Button( L"Scene Viewer", FillWidth(), DefaultHeight() ), "SceneViewer" );
	AddControl( new Button( L"Input Browser", FillWidth(), DefaultHeight() ), "InputBrowser" );
	AddControl( new Button( L"Script Editor", FillWidth(), DefaultHeight() ), "ScriptEditor" );
	AddControl( new Button( L"Log Viewer", FillWidth(), DefaultHeight() ), "LogViewer" );
	AddControl( new Button( L"Quit", FillWidth(), DefaultHeight() ), "Quit" );
	Create( L"Engine Main", x, y, nCmdShow );

	/*
	// Disable input browser button if we don't have one.
	if ( game->GetInputManager() == nullptr )
	{
		EnableWindow( GetDlgItem( builder.GetHandle(), IDC_BUTTON_INPUTBROWSER ), false );
	}
	*/
}

me::IGame * EngineMain::GetGame() const
{
	return m_game;
}

using namespace ui;

void EngineMain::OpenResourceBrowser()
{
	RECT rect{};
	GetWindowRect( rect );
	int x = rect.right;
	int y = rect.top;
	if ( !m_resourceBrowser )
	{
		m_resourceBrowser.reset( new meedr::ResourceBrowser( this, SW_SHOWDEFAULT, x + m_openChildren * 34, y + m_openChildren * 34, m_game ) );
		m_openChildren++;
	}
	else
	{
		m_resourceBrowser->ShowWindow( SW_RESTORE );
		m_resourceBrowser->SetForegroundWindow();
	}							  
}	

void EngineMain::OpenInputBrowser()
{
	RECT rect{};
	GetWindowRect( rect );
	int x = rect.right;
	int y = rect.top;
	if ( !m_inputBrowser )
	{
		m_inputBrowser.reset( new InputBrowser( this, SW_SHOWDEFAULT, x + m_openChildren * 34, y + m_openChildren * 34, m_game ) );
		m_openChildren++;
	}
	else
	{
		m_inputBrowser->ShowWindow( SW_RESTORE );
		m_inputBrowser->SetForegroundWindow();
	}
}
void EngineMain::OpenScriptEditor( unify::Path source )
{
	RECT rect{};
	GetWindowRect( rect );
	int x = rect.right;
	int y = rect.top;
	if ( ! m_scriptEditor )
	{
		m_scriptEditor.reset( new ScriptEditor( this, SW_SHOWDEFAULT, x + m_openChildren * 34, y + m_openChildren * 34, m_game ) );
		m_openChildren++;
	}
	else
	{
		m_scriptEditor->ShowWindow( SW_RESTORE );
		m_scriptEditor->SetForegroundWindow();
	}

	ScriptEditor* scriptEditor = dynamic_cast<ScriptEditor*>(m_scriptEditor.get());
	scriptEditor->LoadFile( source );
}

void EngineMain::OpenSceneViewer()
{	
	RECT rect{};
	GetWindowRect( rect );
	int x = rect.right;
	int y = rect.top;
	if ( ! m_sceneViewer )
	{
		m_sceneViewer.reset( new SceneViewer( this, SW_SHOWDEFAULT, x + m_openChildren * 34, y + m_openChildren * 34, m_game ) );
		m_openChildren++;
	}
	else
	{
		m_sceneViewer->ShowWindow( SW_RESTORE );
		m_sceneViewer->SetForegroundWindow();
	}
}

void EngineMain::OpenLogViewer()
{	
	RECT rect{};
	GetWindowRect( rect );
	int x = rect.right;
	int y = rect.top;
	if ( ! m_logViewer )
	{
		m_logViewer.reset( new LogViewer( this, SW_SHOWDEFAULT, x + m_openChildren * 34, y + m_openChildren * 34, m_game ) );
		m_openChildren++;
	}
	else
	{
		m_logViewer->ShowWindow( SW_RESTORE );
		m_logViewer->SetForegroundWindow();
	}
}

IResult * EngineMain::OnControlCommand( ControlMessage message )
{
	if ( message.IsFor( "Quit" ) )
	{
		m_game->Quit();
		return new Result( 0 );
	}
	else if ( message.IsFor( "ResourceBrowser" ) )
	{
		OpenResourceBrowser();
		return new Result( 0 );
	}
	else if ( message.IsFor(  "InputBrowser" ) )
	{
		OpenInputBrowser();
		return new Result( 0 );
	}
	else if ( message.IsFor( "ScriptEditor" ) )
	{
		OpenScriptEditor( unify::Path() );
		return new Result( 0 );
	}
	else if ( message.IsFor( "SceneViewer" ) )
	{	
		OpenSceneViewer();
		return new Result( 0 );
	}
	else if ( message.IsFor( "LogViewer" ) )
	{
		OpenLogViewer();
		return new Result( 0 );
	}
	return new Unhandled();
}

IResult* EngineMain::OnUserMessage( UserMessageData message )
{
	switch ( message.message )
	{
	case RESOURCEBROWSER_CLOSED:
		m_resourceBrowser.reset();
		m_openChildren--;
		return new Result( 0 );
	case INPUTBROWSER_CLOSED:
		m_inputBrowser.reset();
		m_openChildren--;
		return new Result( 0 );
	case SCRIPTEDITOR_CLOSED:
		m_scriptEditor.reset();
		m_openChildren--;
		return new Result( 0 );
	case SCENEVIEWER_CLOSED:
		m_sceneViewer.reset();
		m_openChildren--;
		return new Result( 0 );	
	case LOGVIEWER_CLOSED:
		m_logViewer.reset();
		m_openChildren--;
		return new Result( 0 );
	case SCRIPTEDITOR_OPEN:
		OpenScriptEditor( message.params.lParam ? unify::Path( (char*)message.params.lParam ) : unify::Path() );
		return new Result( 0 );
	}
	return new Unhandled();
}