#pragma once
#include <Windows.h>

extern "C"
{
	__declspec(dllexport) bool Startup( HWND hwnd );
	__declspec(dllexport) void Shutdown( HWND hwnd );
	__declspec(dllexport) bool IsShutdown( HWND hwnd );
}