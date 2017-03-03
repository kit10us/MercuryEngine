// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/IOS.h>
#include <me/IGame.h>
#include <me/IRenderer.h>
#include <me/Display.h>
#include <list>
#include <vector>

// Undefine Microsoft clashing defines.
#ifdef CreateWindow
#undef CreateWindow
#endif

#ifdef GetCommandLine
#undef GetCommandLine
#endif

namespace meedr
{
	HWND CreateScriptEditor( me::IGame * game, HINSTANCE hInstance, HWND parentHandle, int nCmdShow, int x, int y );
}
