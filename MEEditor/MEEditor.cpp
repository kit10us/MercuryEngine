// DXILuaDLL.cpp : Defines the exported functions for the DLL application.
//

#include "MEEditor.h"
#include <meedr/EngineMain.h>
#include <me/Game.h>
#include <Windows.h>

meedr::ui::IWindow::ptr s_engineMain;

extern "C" __declspec(dllexport) bool DXILoader( me::IGame * game, const qxml::Document * document )
{
	s_engineMain.reset();
	s_engineMain.reset( new meedr::EngineMain( game ) );
	return true;
}

