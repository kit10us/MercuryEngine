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

#define WM_RESOURCEBROWSER_CLOSED	(WM_USER+0)

namespace meedr
{
	HWND CreateEngineMain( me::IGame * game );
}
