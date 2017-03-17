// MELuaDLL.cpp : Defines the exported functions for the DLL application.
//

#include "MEEditor.h"
#include <meedr/SceneViewer.h>
#include <me/Game.h>
#include <Windows.h>

ui::IWindow::ptr s_engineMain;

extern "C" __declspec(dllexport) bool MELoader( me::IGame * game, const qxml::Element * element )
{
	s_engineMain.reset( new meedr::SceneViewer( game ) );
	return true;
}

