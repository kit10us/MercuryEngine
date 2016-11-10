// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/Game.h>
#include <shellapi.h>

extern "C" me::Game * GetGameInstance();

extern "C"
LRESULT CALLBACK WndProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam )
{

	//static bool trackingMouse = false;
	//static dxi::core::Game & game = *GetGameInstance();

	//dxi::win::WindowsOS * os = dynamic_cast< dxi::win::WindowsOS * >( game.GetOS() );
	//return os->WndProc( handle, message, wParam, lParam );

	auto result = GetGameInstance()->GetOS()->Feed( "OS", (void*)&me::os::win::OSFood( (me::os::win::HWnd)handle, message, (me::os::win::WParam)wParam, (me::os::win::LParam)lParam ) );
	return (LRESULT)result;

	//return DefWindowProc( handle, message, wParam, lParam );
}

/*
extern "C"
LRESULT CALLBACK WndProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam )
{
	static bool trackingMouse = false;
	static dxi::core::Game & game = *GetGameInstance();
	static dxi::win::WindowsOS * windowsOS = dynamic_cast< dxi::win::WindowsOS*>( game.GetOS() );

	return windowsOS->WndProc( handle, message, wParam, lParam );
}
*/