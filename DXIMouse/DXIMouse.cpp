// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include "DXIMouse.h"
#include <dximouse/Mouse.h>
#include <me/game/Game.h>

void DeleterMouse( dximouse::Mouse * mouse )
{
	delete mouse;
}

extern "C" __declspec(dllexport) bool MELoader( me::game::IGame * gameInstance, const qxml::Element * element );
__declspec(dllexport) bool MELoader( me::game::IGame * gameInstance, const qxml::Element * element )
{
	gameInstance->GetInputManager()->AddInputDevice( me::input::IInputDevice::ptr( new dximouse::Mouse( gameInstance ), DeleterMouse ) );

	return true;
}

