// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/EngineMain.h>
#include <meedr/ResourceBrowser.h>
#include <meedr/ui/Builder.h>

static HWND s_parentWindow;
static HWND s_windowHwnd;
#define IDC_COMBOBOX_TYPES 0x001
#define IDC_LISTBOX_RESOURCES 0x002

void UpdateResourceTypes( me::IGame * game, HWND hWnd )
{
	// Clear contents...
	//SendMessageA( hWnd, CB_RESETCONTENT, 0, 0 );

	// Fill in resource types...
	for ( size_t i = 0; i < game->GetResourceHub().GetTypeCount(); i++ )
	{			  
		std::string name = game->GetResourceHub().GetTypeName( i ).c_str();
		SendMessageA( hWnd, CB_ADDSTRING, 0, (LPARAM)name.c_str() );
	}

	// Select first type:
	SendMessageA( hWnd, CB_SETCURSEL, 0, 0 );
}

void UpdateResourceList( me::IGame * game, HWND hWnd )
{
	// Clear contents...
	SendMessageA( hWnd, (UINT)LB_RESETCONTENT, (WPARAM)0, (LPARAM)0 );

	HWND hwndCombo = GetDlgItem( s_windowHwnd, IDC_COMBOBOX_TYPES );
	size_t typeIndex = SendMessageA( hwndCombo, CB_GETCURSEL, 0, 0 );

	// Fill in resource types...
	for ( size_t i = 0; i < game->GetResourceHub().GetManagerRaw( typeIndex )->Count(); i++ )
	{			  
		std::string name = game->GetResourceHub().GetManagerRaw( typeIndex )->GetResourceName( i );
		SendMessageA( hWnd, LB_ADDSTRING, 0, (LPARAM)name.c_str() );
	}

	// Select first type:
	SendMessageA( hWnd, LB_SETCURSEL, 0, 0 );
}

LRESULT CALLBACK Browser_WndProcChild( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	static me::IGame * game;
	switch ( message )
	{
	case WM_CREATE:
	{			
		CREATESTRUCT * createStruct = (CREATESTRUCT*)lParam;
		game = (me::IGame*)createStruct->lpCreateParams;
		break;
	}
	case WM_DESTROY:
	{
		SendMessageA( s_parentWindow, WM_RESOURCEBROWSER_CLOSED, 0, 0 );
		break;
	}
	case WM_COMMAND:
	{
		auto control = LOWORD( wParam );
		auto controlMessage = HIWORD( wParam );
		switch ( control )
		{
		case IDC_COMBOBOX_TYPES:
		{
			switch ( controlMessage )
			{
			case CBN_SELCHANGE:
			{
				HWND hWndList = GetDlgItem( hWnd, IDC_LISTBOX_RESOURCES );
				UpdateResourceList( game, hWndList );
				break;
			}
			}
			break;
		}
		}
		break;
	}

	}

	return DefWindowProc( hWnd, message, wParam, lParam );
}

HWND meedr::CreateResourceBrowser( me::IGame * game, HINSTANCE hInstance, HWND parentHandle, int nCmdShow, int x, int y )
{
	s_parentWindow = parentHandle;

	// Regardless of windowed or not, we need a window...
	WNDCLASS wc{};

	if ( !GetClassInfo( hInstance, L"ResourceBrowserWndClass", &wc ) )
	{
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = (WNDPROC)Browser_WndProcChild;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon( (HINSTANCE)NULL, IDI_APPLICATION );
		wc.hbrBackground = (HBRUSH)GetSysColorBrush( COLOR_3DFACE );
		wc.hCursor = LoadCursor( (HINSTANCE)NULL, IDC_ARROW );
		wc.lpszMenuName = 0;
		wc.lpszClassName = L"ResourceBrowserWndClass";
		if ( !RegisterClass( &wc ) )
		{
			throw std::exception( "Failed to register window class!" );
		}
	}

	ui::Builder builder;
	builder.AddContainer( new ui::container::StackPanel( ui::container::Stack::Horizontal, 540, 440 ) );
	builder.AddContainer( new ui::container::StackPanel( ui::container::Stack::Vertical, ui::FillWidth(), ui::FillHeight() ) );
	builder.AddControl( new ui::Static( L"Type:", 50, ui::DefaultHeight() ) );
	builder.AddControl( new ui::Combobox( ui::FillWidth(), ui::DefaultHeight(), IDC_COMBOBOX_TYPES ) );
	builder.AddControl( new ui::Listbox( ui::FillWidth(), ui::FillHeight(), IDC_LISTBOX_RESOURCES ) );
	s_windowHwnd = builder.Create( s_parentWindow, hInstance, L"ResourceBrowserWndClass", L"Resource Browser", x, y, game );
	
	ShowWindow( s_windowHwnd, nCmdShow );

	UpdateResourceTypes( game, GetDlgItem( s_windowHwnd, IDC_COMBOBOX_TYPES ) );
	UpdateResourceList( game, GetDlgItem( s_windowHwnd, IDC_LISTBOX_RESOURCES ) );

	return s_windowHwnd;
}

