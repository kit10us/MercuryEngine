// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/EngineMain.h>
#include <meedr/ui/Window.h>
#include <meedr/ResourceBrowser.h>
#include <meedr/InputBrowser.h>
#include <meedr/ScriptEditor.h>
#include <meedr/SceneViewer.h>

using namespace meedr;
using namespace ui;

EngineMain::EngineMain( me::IGame * game )
	: Window( (HWND)game->GetOSParameters().hWnd, L"EngineMainClass" )
	, m_game{ game }
	, m_openChildren{ 0 }
{
	int nCmdShow = game->GetOSParameters().nCmdShow;
	RECT rect{};
	GetWindowRect( GetParentHandle(), &rect );
	int x = rect.right;
	int y = rect.top;

	AddContainer( new container::StackPanel( container::Stack::Vertical, 240, SizeToContentHeight() ) );
	AddControl( new Button( L"Pause", FillWidth(), DefaultHeight() ), "Pause" );
	AddControl( new Button( L"Resource Browser", FillWidth(), DefaultHeight() ), "ResourceBrowser" );
	AddControl( new Button( L"Scene Viewer", FillWidth(), DefaultHeight() ), "SceneViewer" );
	AddControl( new Button( L"Input Browser", FillWidth(), DefaultHeight() ), "InputBrowser" );
	AddControl( new Button( L"Script Editor", FillWidth(), DefaultHeight() ), "ScriptEditor" );
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

IResult * EngineMain::OnControlCommand( ControlMessage message )
{
	if ( unify::StringIs( message.name, "Quit" ) )
	{
		m_game->Quit();
		return new Result( 0 );
	}
	else if ( unify::StringIs( message.name, "Pause" ) )
	{
		m_game->SetUpdateEnabled( !m_game->GetUpdateEnabled() );
		SetDlgItemText( GetHandle(), message.controlId, m_game->GetUpdateEnabled() ? L"Pause" : L"Resume" );
		return new Result( 0 );
	}
	else if ( unify::StringIs( message.name, "ResourceBrowser" ) )
	{
		RECT rect{};
		GetWindowRect( GetHandle(), &rect );
		int x = rect.right;
		int y = rect.top;
		if ( !m_resourceBrowser )
		{
			m_resourceBrowser.reset( new meedr::ResourceBrowser( GetHandle(), SW_SHOWDEFAULT, x + m_openChildren * 34, y + m_openChildren * 34, m_game ) );
			m_openChildren++;
		}
		else
		{
			ShowWindow( m_resourceBrowser->GetHandle() , SW_RESTORE );
			SetForegroundWindow( m_resourceBrowser->GetHandle() );
			FlashWindow( m_resourceBrowser->GetHandle(), true );
			m_resourceBrowser->MoveWindow( x + m_openChildren * 34, y + m_openChildren * 34, true );
		}
		m_openChildren++;
		return new Result( 0 );
	}
	else if ( unify::StringIs( message.name, "InputBrowser" ) )
	{
		RECT rect{};
		GetWindowRect( GetHandle(), &rect );
		int x = rect.right;
		int y = rect.top;
		if ( !m_inputBrowser )
		{
			m_inputBrowser.reset( new InputBrowser( GetHandle(), SW_SHOWDEFAULT, x + m_openChildren * 34, y + m_openChildren * 34, m_game ) );
			m_openChildren++;
			return new Result( 0 );
		}
		else
		{
			ShowWindow( m_inputBrowser->GetHandle(), SW_RESTORE );
			SetForegroundWindow( m_inputBrowser->GetHandle() );
			FlashWindow( m_inputBrowser->GetHandle(), true );

			m_inputBrowser->MoveWindow(  x, y, true );
		}
		return new Result( 0 );
	}
	else if ( unify::StringIs( message.name, "ScriptEditor" ) )
	{
		RECT parentRect{};
		GetWindowRect( GetHandle(), &parentRect );
		int x = parentRect.right;
		int y = parentRect.top;
		if ( ! m_scriptEditor )
		{
			m_scriptEditor.reset( new ScriptEditor( GetHandle(), SW_SHOWDEFAULT, x + m_openChildren * 34, y + m_openChildren * 34, m_game ) );
			m_openChildren++;
		}
		else
		{
			ShowWindow( m_scriptEditor->GetHandle(), SW_RESTORE );
			SetForegroundWindow( m_scriptEditor->GetHandle() );
			FlashWindow( m_scriptEditor->GetHandle(), true );
			m_scriptEditor->MoveWindow( x, y, true );
		}
		return new Result( 0 );
	}
	else if ( unify::StringIs( message.name, "SceneViewer" ) )
	{
		RECT parentRect{};
		GetWindowRect( GetHandle(), &parentRect );
		int x = parentRect.right;
		int y = parentRect.top;
		if ( ! m_sceneViewer )
		{
			m_sceneViewer.reset( new SceneViewer( GetHandle(), SW_SHOWDEFAULT, x + m_openChildren * 34, y + m_openChildren * 34, m_game ) );
			m_openChildren++;
		}
		else
		{
			ShowWindow( m_sceneViewer->GetHandle(), SW_RESTORE );
			SetForegroundWindow( m_sceneViewer->GetHandle() );
			FlashWindow( m_sceneViewer->GetHandle(), true );
			m_sceneViewer->MoveWindow( x, y, true );
		}
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
		m_inputBrowser.reset();
		m_openChildren--;
		return new Result( 0 );
	case SCENEVIEWER_CLOSED:
		m_sceneViewer.reset();
		m_openChildren--;
		return new Result( 0 );
	}
	return new Unhandled();
}