// DXILuaDLL.cpp : Defines the exported functions for the DLL application.
//

#include "DXIKeyboard.h"
#include <dxikeyboard/Keyboard.h>
#include <me/IGame.h>

void DeleterKeyboard( dxikeyboard::Keyboard * keyboard )
{
	delete keyboard;
}

extern "C" __declspec(dllexport) bool DXILoader( me::IGame * game, const qxml::Document * doc );
__declspec(dllexport) bool DXILoader( me::IGame * game, const qxml::Document * document )
{
	game->GetInputManager()->AddInputSource( me::IInputSource::ptr( new dxikeyboard::Keyboard( game ), DeleterKeyboard ) );

	return true;
}

