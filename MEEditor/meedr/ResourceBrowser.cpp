// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/EngineMain.h>
#include <meedr/ResourceBrowser.h>
#include <meedr/ui/Window.h>

using namespace meedr;
using namespace ui;


ResourceBrowser::ResourceBrowser( HWND parentHandle, int nCmdShow, int x, int y, me::IGame * game )
	: Window( parentHandle, L"ResourceBrowserWndClass" )
	, m_game{ game }
{
	AddContainer( new ui::container::StackPanel( ui::container::Stack::Vertical, 540, 440 ) );
	AddControl( new ui::Static( L"Type:", 50, ui::DefaultHeight() ) );
	AddControl( new ui::Combobox( ui::FillWidth(), ui::DefaultHeight() ), "Types" );
	AddControl( new ui::Listbox( ui::FillWidth(), ui::FillHeight() ), "Resources" );
	Create( L"Resource Browser", x, y, nCmdShow );
}

void ResourceBrowser::UpdateResourceTypes( HWND hWnd )
{
	// Clear contents...
	SendMessageA( hWnd, CB_RESETCONTENT, 0, 0 );

	// Fill in resource types...
	for ( size_t i = 0; i < m_game->GetResourceHub().GetTypeCount(); i++ )
	{			  
		std::string name = m_game->GetResourceHub().GetTypeName( i ).c_str();
		SendMessageA( hWnd, CB_ADDSTRING, 0, (LPARAM)name.c_str() );
	}

	// Select first type:
	SendMessageA( hWnd, CB_SETCURSEL, 0, 0 );
}

void ResourceBrowser::UpdateResourceList( HWND hWnd )
{
	// Clear contents...
	SendMessageA( hWnd, (UINT)LB_RESETCONTENT, (WPARAM)0, (LPARAM)0 );

	HWND hwndCombo = GetDlgItem( GetHandle(), GetControl( "Types" ) );
	size_t typeIndex = SendMessageA( hwndCombo, CB_GETCURSEL, 0, 0 );

	// Fill in resource types...
	for ( size_t i = 0; i < m_game->GetResourceHub().GetManagerRaw( typeIndex )->Count(); i++ )
	{			  
		std::string name = m_game->GetResourceHub().GetManagerRaw( typeIndex )->GetResourceName( i );
		SendMessageA( hWnd, LB_ADDSTRING, 0, (LPARAM)name.c_str() );
	}

	// Select first type:
	SendMessageA( hWnd, LB_SETCURSEL, 0, 0 );
}

IResult * ResourceBrowser::OnCreate( Params params )
{
	return new Result( 0 );
}

IResult * ResourceBrowser::OnDestroy( Params params )
{
	SendMessageA( GetParentHandle(), WM_USER + RESOURCEBROWSER_CLOSED, 0, 0 );
	return new Result( 0 );
}

IResult * ResourceBrowser::OnAfterCreate( Params )
{
	UpdateResourceTypes( GetDlgItem( GetHandle(), GetControl( "Types" ) ) );
	UpdateResourceList( GetDlgItem( GetHandle(), GetControl( "Resources" ) ) );
	return new Result( 0 );
}

IResult * ResourceBrowser::OnControlCommand( ControlMessage message )
{
	if ( message.IsFor( "Types" ) )
	{
		switch ( message.message )
		{
		case CBN_SELCHANGE:
		{
			HWND hWndList = GetDlgItem( GetHandle(), GetControl( "Resources" ) );
			UpdateResourceList( hWndList );
			break;
		}
		}
	}	  
	return new Result( 0 );
}			
