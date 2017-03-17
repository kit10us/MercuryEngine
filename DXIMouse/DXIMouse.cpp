// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include "DXIMouse.h"
#include <dximouse/Mouse.h>
#include <me/Game.h>

void DeleterMouse( dximouse::Mouse * mouse )
{
	delete mouse;
}

extern "C" __declspec(dllexport) bool MELoader( me::IGame * game, const qxml::Element * element );
__declspec(dllexport) bool MELoader( me::IGame * game, const qxml::Element * element )
{
	game->GetInputManager()->AddInputSource( me::input::IInputSource::ptr( new dximouse::Mouse( game ), DeleterMouse ) );

	return true;
}

