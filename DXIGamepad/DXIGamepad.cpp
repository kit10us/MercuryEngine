// DXILuaDLL.cpp : Defines the exported functions for the DLL application.
//

#include "DXIGamepad.h"
#include <dxi/win/DXILib.h>
#include <dxigp/Gamepad.h>
#include <dxi/core/IGame.h>


void DeleterGamepad( dxigp::Gamepad * gamepad )
{
	delete gamepad;
}

extern "C" __declspec(dllexport) bool DXILoader( dxi::core::IGame * game, const qxml::Document * doc );
__declspec(dllexport) bool DXILoader( dxi::core::IGame * game, const qxml::Document * document )
{
	game->GetInputManager()->AddInputSource( dxi::input::IInputSource::ptr( new dxigp::Gamepad( game ), DeleterGamepad ) );

	return true;
}

