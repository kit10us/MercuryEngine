// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/win/WindowsOS.h>
#include <shellapi.h>

extern "C" dxi::core::Game * GetGameInstance();

extern "C"
LRESULT CALLBACK WndProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam )
{
	static bool trackingMouse = false;
	static dxi::core::Game & game = *GetGameInstance();

	dxi::win::WindowsOS * os = dynamic_cast< dxi::win::WindowsOS * >( game.GetOS() );
	return os->WndProc( handle, message, wParam, lParam );

	return DefWindowProc( handle, message, wParam, lParam );
}
