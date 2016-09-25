#pragma once
#include <Windows.h>

//#define EXPORT __stdcall
#define EXPORT __cdecl

extern "C"
{
	__declspec(dllexport) bool EXPORT Startup( HWND hwnd );
	__declspec(dllexport) bool EXPORT Tick( HWND hwnd );
	__declspec(dllexport) void EXPORT Shutdown( HWND hwnd );
	__declspec(dllexport) bool EXPORT IsShutdown( HWND hwnd );
}