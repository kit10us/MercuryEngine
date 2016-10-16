// DXILuaDLL.cpp : Defines the exported functions for the DLL application.
//

#include "DXIMouse.h"
#include <dxi/win/DXILib.h>
#include <dxi/win/DXRenderer.h>
#include <dximouse/Mouse.h>

void DeleterMouse( dximouse::Mouse * mouse )
{
	delete mouse;
}

__declspec(dllexport) bool DXILoader( dxi::core::Game * game, const qxml::Document * document )
{
	game->GetInputManager()->AddInputSource( dxi::input::IInputSource::ptr( new dximouse::Mouse( game ), DeleterMouse ) );

	return true;
}

