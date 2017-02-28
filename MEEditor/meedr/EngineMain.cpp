// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/EngineMain.h>
#include <meedr/ui/Builder.h>
#include <meedr/ResourceBrowser.h>

#define IDC_BUTTON_PAUSE 0x001
#define IDC_BUTTON_RESOURCEBROWSER 0x002

LRESULT CALLBACK WndProcChild( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	static me::IGame * game;
	static CREATESTRUCT createStruct;
	static HWND resourceBrowserHwnd;

	switch ( message )
	{
	case WM_CREATE:
	{
		createStruct = *(CREATESTRUCT*)lParam;
		game = (me::IGame*)createStruct.lpCreateParams;
		break;
	}
	case WM_COMMAND:
	{
		auto control = LOWORD( wParam );
		switch ( control )
		{
		case IDC_BUTTON_PAUSE:
			game->SetUpdateEnabled( !game->GetUpdateEnabled() );
			SetDlgItemText( hWnd, IDC_BUTTON_PAUSE, game->GetUpdateEnabled() ? L"Pause" : L"Resume" );
			break;
		case IDC_BUTTON_RESOURCEBROWSER:
		{
			RECT rect{};
			GetWindowRect( hWnd, &rect );
			int x = rect.right;
			int y = rect.top;
			if ( !resourceBrowserHwnd )
			{
				resourceBrowserHwnd = meedr::CreateResourceBrowser( game, createStruct.hInstance, hWnd, SW_SHOWDEFAULT, x, y );
			}
			else
			{
				ShowWindow( resourceBrowserHwnd, SW_RESTORE );
				SetForegroundWindow( resourceBrowserHwnd );
				FlashWindow( resourceBrowserHwnd, true );

				RECT resourceBrowserRect{};
				GetWindowRect( resourceBrowserHwnd, &resourceBrowserRect );
				int width = resourceBrowserRect.right - resourceBrowserRect.left;
				int height = resourceBrowserRect.bottom - resourceBrowserRect.top;
				MoveWindow( resourceBrowserHwnd, x, y, width, height, 1 );
			}
			break;
		}
		}
		break;
	}
	case WM_RESOURCEBROWSER_CLOSED:
		resourceBrowserHwnd = 0;
		break;
	}

	return DefWindowProc( hWnd, message, wParam, lParam );
}

HWND meedr::CreateEngineMain( me::IGame * game )
{
	//, GetHInstance(), handle, m_osParameters.nCmdShow, windowRect.left + newWindowWidth, windowRect.top

	HINSTANCE hInstance = (HINSTANCE)game->GetOSParameters().hInstance;
	HWND parentHandle = (HWND)game->GetOSParameters().hWnd;
	int nCmdShow = game->GetOSParameters().nCmdShow;
	RECT rect{};
	GetWindowRect( parentHandle, &rect );
	int x = rect.right;
	int y = rect.top;

	// Regardless of windowed or not, we need a window...
	WNDCLASS wc{};
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProcChild;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon( (HINSTANCE)NULL, IDI_APPLICATION );
	wc.hbrBackground = (HBRUSH)GetSysColorBrush( COLOR_3DFACE );
	wc.hCursor = LoadCursor( (HINSTANCE)NULL, IDC_ARROW );
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"SecondWndClass"; 
	if( !RegisterClass( &wc ) )
	{
		throw std::exception( "Failed to register window class!" );
	}

	ui::Builder builder;
	builder.AddContainer( new ui::container::StackPanel( ui::container::Stack::Vertical, 240, 300 ) );
	builder.AddControl( new ui::Button( L"Pause", ui::FillWidth(), ui::DefaultHeight(), IDC_BUTTON_PAUSE ) );
	builder.AddControl( new ui::Button( L"Resource Browser", ui::FillWidth(), 26, IDC_BUTTON_RESOURCEBROWSER ) );
	builder.Create( parentHandle, hInstance, L"SecondWndClass", L"Engine Main", x, y, game );

	ShowWindow( builder.GetHandle(), nCmdShow );

	return builder.GetHandle();
}