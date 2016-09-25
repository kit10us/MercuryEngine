// ISSUE: 1>main.obj : error LNK2019: unresolved external symbol _D3D11CreateDeviceAndSwapChain@48 referenced in function _WinMain@16
// RESOL: 

#include <windows.h>
#include <cstdlib>
#include <string>
#include <tchar.h>
#include <fstream>
#include <cassert>
#include <string>
#include <iostream>
#include <atlbase.h>

// Undefine Microsoft pervasive definitions...

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif


LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	hPrevInstance; // NOT USED
	lpCmdLine; // NOT USED

	HRESULT result = S_OK;

	// New DLL loading...

	HMODULE module = LoadLibraryA( "DX11DLL.dll" );
	if ( module == nullptr )
	{
		return -1;
	}


	typedef bool (__cdecl *Startup)( HWND hwnd );
	typedef bool (__cdecl *Tick )( HWND hwnd );
	typedef void (__cdecl *Shutdown)( HWND hwnd );
	typedef bool (__cdecl *IsShutdown)( HWND hwnd );

	Startup startupProc = (Startup)GetProcAddress( module, "Startup" );
	Tick tickProc = (Tick)GetProcAddress( module, "Tick" );
	Shutdown shutdownProc = (Shutdown)GetProcAddress( module, "Shutdown" );
	IsShutdown isShutdownProc = (IsShutdown)GetProcAddress( module, "IsShutdown" );

	if ( !startupProc || !shutdownProc || !isShutdownProc )
	{
		return -1;
	}

#pragma region Create window...
	int width = 800;
	int height = 600;

	WNDCLASSEX wndClassEx;
	ZeroMemory( &wndClassEx, sizeof( WNDCLASSEX ) );
	wndClassEx.cbSize = sizeof( WNDCLASSEX );
	wndClassEx.style = CS_HREDRAW | CS_VREDRAW;
	wndClassEx.lpfnWndProc = WndProc;
	wndClassEx.cbClsExtra = 0;
	wndClassEx.cbWndExtra = 0;
	wndClassEx.hInstance = hInstance;
	wndClassEx.hIcon = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_APPLICATION ) );
	wndClassEx.hCursor = LoadCursor( 0, IDC_ARROW );
	wndClassEx.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wndClassEx.lpszMenuName = 0;
	wndClassEx.lpszClassName = TEXT("MainWndClass");
	wndClassEx.hIconSm = LoadIcon( wndClassEx.hInstance, MAKEINTRESOURCE( IDI_APPLICATION ) );

	if ( ! RegisterClassEx( &wndClassEx ) )
	{
		MessageBoxA( 0, "Failed to register class!", "FAILURE", 0 );
		return 1;
	}

	HWND hWnd = CreateWindow( TEXT("MainWndClass"), TEXT("Basic Window"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, (HWND)0, (HMENU)0, hInstance, 0 );
	if ( hWnd == 0 )
	{
		MessageBox( 0, TEXT("Failed to create window!"), TEXT("Failure"), 0 );
		return 1;
	}

	ShowWindow( hWnd, nCmdShow );
	UpdateWindow( hWnd );
#pragma endregion

	if( !startupProc( hWnd ) )
	{
		return -1;
	}

#pragma region The main loop...
	MSG msg = MSG();
	const bool forever = true;
	while( forever )
	{
		// Allow processing of all messages...
		bool quitRequest = false;
		while( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) == 1 )
		{
			if ( msg.message == WM_QUIT )
			{
				quitRequest = true;
				break;
			}
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		if ( quitRequest )
		{
			break;
		}

		tickProc( hWnd );
	}
#pragma endregion

	shutdownProc( hWnd );
	while( !IsShutdown( hWnd ) ) Sleep( 100 );

	return msg.wParam;
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage( 0 );
		return 0;

	case WM_KEYUP:
		switch( wParam )
		{
		case 'Q':
			PostQuitMessage( 0 );
			return 0;
		}
		break;
	}

	return DefWindowProc( hWnd, message, wParam, lParam );
}