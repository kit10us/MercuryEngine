// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/Game.h>
#include <shellapi.h>

extern "C" me::Game * GetGameInstance();

extern "C"
LRESULT CALLBACK WndProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam )
{
	auto result = GetGameInstance()->GetOS()->Feed( "OS", (void*)&me::os::win::OSFood( (me::os::win::HWnd)handle, message, (me::os::win::WParam)wParam, (me::os::win::LParam)lParam ) );
	return (LRESULT)result;
}
