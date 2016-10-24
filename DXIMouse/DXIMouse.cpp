// DXILuaDLL.cpp : Defines the exported functions for the DLL application.
//

#include "DXIMouse.h"
#include <dxi/win/DXILib.h>
#include <dximouse/Mouse.h>
#include <dxi/core/Game.h>

void DeleterMouse( dximouse::Mouse * mouse )
{
	delete mouse;
}

extern "C" __declspec(dllexport) bool DXILoader( dxi::core::IGame * game, const qxml::Document * doc );
__declspec(dllexport) bool DXILoader( dxi::core::IGame * game, const qxml::Document * document )
{
	game->GetInputManager()->AddInputSource( dxi::input::IInputSource::ptr( new dximouse::Mouse( game ), DeleterMouse ) );

	return true;
}

