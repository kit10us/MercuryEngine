// DXILuaDLL.cpp : Defines the exported functions for the DLL application.
//

#include "DXIMouse.h"
#include <dxi/win/DXILib.h>
#include <dximouse/Mouse.h>
#include <me/Game.h>

void DeleterMouse( dximouse::Mouse * mouse )
{
	delete mouse;
}

extern "C" __declspec(dllexport) bool DXILoader( me::IGame * game, const qxml::Document * doc );
__declspec(dllexport) bool DXILoader( me::IGame * game, const qxml::Document * document )
{
	game->GetInputManager()->AddInputSource( me::IInputSource::ptr( new dximouse::Mouse( game ), DeleterMouse ) );

	return true;
}

