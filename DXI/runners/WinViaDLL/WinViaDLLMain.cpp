#include <Windows.h>
#include <exception>
#include <string>

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_KEYUP:
		switch( wParam )
		{
		case WM_QUIT:
			break;

		case WM_CLOSE: // Fall through to WM_DESTROY...
		case WM_DESTROY:
		case VK_ESCAPE:
			PostQuitMessage( 0 );
			return 0;
		}
		break;
	}
	return DefWindowProc( hwnd, msg, wParam, lParam );
}

int WINAPI WinMain( HINSTANCE instance, HINSTANCE prevInstance, char * cmdLine, int cmdShow )
{
	try
	{
		HMODULE module = LoadLibraryA( "DXIDLL.dll" );

		typedef bool ( __cdecl *StartupProc )(HWND, char *);
		typedef void ( __cdecl *TickProc )( HWND);
		typedef void ( __cdecl *ShutdownProc )( HWND);
									
		StartupProc Startup = (StartupProc)GetProcAddress( module, "Startup" );
		TickProc Tick = (TickProc)GetProcAddress( module, "Tick" );
		ShutdownProc Shutdown = (ShutdownProc)GetProcAddress( module, "Shutdown" );

		WNDCLASS wc;
		memset( &wc, 0, sizeof( wc ) );
		wc.style = 0;
		wc.lpfnWndProc = (WNDPROC)WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = instance;
		wc.hIcon = LoadIcon( (HINSTANCE)NULL, IDI_APPLICATION );
		wc.hCursor = LoadCursor( (HINSTANCE)NULL, IDC_ARROW );
		//wc.hbrBackground = GetStockObject(WHITE_BRUSH); 
		wc.lpszMenuName = L"MainMenu";
		wc.lpszClassName = L"MainWndClass";

		if( !RegisterClass( &wc ) )
		{
			throw "Failed to register window class!";
		}

		unsigned int width = 800;
		unsigned int height = 640;

		HWND hWnd = CreateWindow( L"MainWndClass", L"Sample", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			width, height, (HWND)0, (HMENU)0, instance, (LPVOID)0 );

		if( ! hWnd )
		{
			throw "Failed to create window!";
		}

		ShowWindow( hWnd, cmdShow );
		UpdateWindow( hWnd );

		Startup( hWnd, "setup_luago.xml" );

		MSG msg = MSG();
		const bool forever = true;
		while( forever )
		{
			while ( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) == 1 )
			{
				if ( IsDialogMessage( hWnd, &msg ) )
				{
					if( msg.message == WM_QUIT )
					{
						break;
					}
					TranslateMessage( &msg );
					DispatchMessage( &msg );
				}
			}

			Tick( hWnd );
		}
	}
	catch( std::exception & ex )
	{
		std::string text = "Exception:\n" + std::string( ex.what() );
		MessageBoxA( 0, text.c_str(), "Error", MB_OK );
	}
	catch( ... )
	{
		MessageBoxA( 0, "Unknown error!", "Error", MB_OK );
	}

	return 0;
}

