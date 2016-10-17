// DXILuaDLL.cpp : Defines the exported functions for the DLL application.
//

#include "DXIGamepad.h"
#include <dxi/win/DXILib.h>
#include <dxigp/Gamepad.h>

void DeleterGamepad( dxigp::Gamepad * gamepad )
{
	delete gamepad;
}

__declspec(dllexport) bool DXILoader( dxi::core::Game * game, const qxml::Document * document )
{
	game->GetInputManager()->AddInputSource( dxi::input::IInputSource::ptr( new dxigp::Gamepad( game ), DeleterGamepad ) );

	return true;
}

