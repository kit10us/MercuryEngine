// DXILuaDLL.cpp : Defines the exported functions for the DLL application.
//

#include "MEEditor.h"
#include <meedr/EngineMain.h>
#include <me/Game.h>
#include <Windows.h>

static HWND s_secondWindow;

extern "C" __declspec(dllexport) bool DXILoader( me::IGame * game, const qxml::Document * doc );
__declspec(dllexport) bool DXILoader( me::IGame * game, const qxml::Document * document )
{
	s_secondWindow = meedr::CreateEngineMain( game );
	return true;
}

