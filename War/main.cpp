#include "War.h"

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_KEYUP:
		switch( wParam )
		{
		case VK_ESCAPE:
			PostQuitMessage( 0 );
			return 1;
		}
		break;
	}
	return DefWindowProc( hwnd, msg, wParam, lParam );
}

int WINAPI WinMain( HINSTANCE instance, HINSTANCE prevInstance, char * cmdLine, int cmdShow )
{
	try
	{
		WarApp war( instance, cmdLine, cmdShow, WndProc );
		war.Go();
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

