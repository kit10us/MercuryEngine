// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once
#include <windows.h>

extern "C"
{
	__declspec(dllexport) bool __cdecl Startup( HWND hWnd, char * setup );
	__declspec(dllexport) bool __cdecl Tick( HWND hWnd );
	__declspec(dllexport) void __cdecl Shutdown( HWND hWnd );
}