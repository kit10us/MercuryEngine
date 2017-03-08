// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/EngineMain.h>
#include <meedr/LogViewer.h>
#include <ui/Window.h>

#include <Richedit.h>
#include <ShObjIdl.h>
#include <vector>

using namespace meedr;

LogViewer::LogViewer( IWindow* parent, int nCmdShow, int x, int y, me::IGame * game )
	: Window( parent, L"LogViewerWndClass" )
	, m_game{ game }
{
	using namespace create;
	AddContainer( new container::StackPanel( container::Stack::Horizontal, 640, 480 ) );
	AddControl( (new Richtext( FillWidth(), FillHeight() ))->/*SetMultiline( true )->*/SetReadonly( true )->SetAutoHScroll( true )->SetVScroll( true ), "LogText" );
	Window::Create( L"Log Viewer", x, y, nCmdShow );
}

LogViewer::~LogViewer()
{
	m_game->DetachLogListener( this );
}

using namespace ui;

void LogViewer::Log( std::string text )
{
	Richtext* logText = GetControl< Richtext* >( "LogText" );
	std::string currentText( logText->GetText() );
	logText->SetText( currentText + text  );
}

IResult* LogViewer::OnCreate( Params params )
{
	return new Result( 0 );
}

IResult* LogViewer::OnAfterCreate( Params params )
{
	Richtext* logText = GetControl< Richtext* >( "LogText" );
	logText->SetText( "" );
	m_game->AttachLogListener( this );
	return new Result( 0 );
}

IResult* LogViewer::OnDestroy( Params params )
{
	m_game->DetachLogListener( this );
	GetParent()->SendUserMessage( LOGVIEWER_CLOSED, Params{}  );
	return new Result( 0 );
}

IResult* LogViewer::OnControlCommand( ControlMessage message )
{
	/*
	if ( message.IsFor( "Save" ) )
	{
		return new Result( 0 );
	}
	else if ( message.IsFor( "Load" ) )
	{
		return new Result( 0 );
	}
	*/
	return new Unhandled();
}
