// DXILuaDLL.cpp : Defines the exported functions for the DLL application.
//

#include "DXIGamepad.h"
#include <dxigp/Gamepad.h>
#include <me/IGame.h>


void DeleterGamepad( dxigp::Gamepad * gamepad )
{
	delete gamepad;
}

extern "C" __declspec(dllexport) bool MELoader( me::IGame * game, const qxml::Element * element );
__declspec(dllexport) bool MELoader( me::IGame * game, const qxml::Element * element )
{
	game->GetInputManager()->AddInputSource( me::input::IInputSource::ptr( new dxigp::Gamepad( game ), DeleterGamepad ) );

	return true;
}

