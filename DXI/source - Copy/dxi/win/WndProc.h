// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <windows.h>
#ifdef GetCommandLine
#undef GetCommandLine
#endif // GetCommandLine

extern "C" LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
