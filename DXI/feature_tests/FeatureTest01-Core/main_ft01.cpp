#include <dxi/win/WindowsOS.h>

extern "C"
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	hWnd;
	message;
	wParam;
	lParam;
	return 0;
}

void main( void )
{
	HINSTANCE hInstance = 0;
	HINSTANCE hPrevInstance = 0;
	char * lpszCmdLine = 0;
	int nCmdShow = 0;
	dxi::win::WindowsOS * windowsOS = new dxi::win::WindowsOS( hInstance, hPrevInstance, lpszCmdLine, nCmdShow );
}