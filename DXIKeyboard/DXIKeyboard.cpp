// DXILuaDLL.cpp : Defines the exported functions for the DLL application.
//

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
	game->GetInputManager()->AddInputSource( me::input::IInputSource::ptr( new dxikeyboard::Keyboard( game ), DeleterKeyboard ) );

	return true;
}

