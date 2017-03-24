// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

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
	game->GetInputManager()->AddInputDevice( me::input::IInputDevice::ptr( new dxigp::Gamepad( game ), DeleterGamepad ) );

	return true;
}

