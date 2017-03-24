// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include "DXIKeyboard.h"
#include <dxikeyboard/Keyboard.h>
#include <me/IGame.h>

void DeleterKeyboard( dxikeyboard::Keyboard * keyboard )
{
	delete keyboard;
}

extern "C" __declspec(dllexport) bool MELoader( me::IGame * game, const qxml::Element * element );
__declspec(dllexport) bool MELoader( me::IGame * game, const qxml::Element * element )
{
	game->GetInputManager()->AddInputDevice( me::input::IInputDevice::ptr( new dxikeyboard::Keyboard( game ), DeleterKeyboard ) );

	return true;
}

