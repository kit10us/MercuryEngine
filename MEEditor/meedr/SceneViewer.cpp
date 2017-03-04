// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/EngineMain.h>
#include <meedr/SceneViewer.h>

#include <Richedit.h>
#include <ShObjIdl.h>
#include <vector>

using namespace meedr;
using namespace ui;

SceneViewer::SceneViewer( HWND parent, int nCmdShow, int x, int y, me::IGame * game )
	: Window( parent, L"SceneViewerWndClass" )
	, m_game{ game }
{
	AddContainer( new container::StackPanel( container::Stack::Vertical, 640, 480 ) );
	AddControl( new Treeview( FillWidth(), FillHeight() ), "Tree" );
	Create( L"Scene Viewer", x, y, nCmdShow );
}

IResult* SceneViewer::OnDestroy( Params params )
{
	SendMessageA( GetParentHandle(), WM_USER + SCENEVIEWER_CLOSED, 0, 0 ); 
	return new Result(0);
}
