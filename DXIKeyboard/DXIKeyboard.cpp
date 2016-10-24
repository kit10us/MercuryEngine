// DXILuaDLL.cpp : Defines the exported functions for the DLL application.
//

#include "DXIKeyboard.h"
#include <dxi/win/DXILib.h>
#include <dxikeyboard/Keyboard.h>
#include <dxi/core/IGame.h>

void DeleterKeyboard( dxikeyboard::Keyboard * keyboard )
{
	delete keyboard;
}

extern "C" __declspec(dllexport) bool DXILoader( dxi::core::IGame * game, const qxml::Document * doc );
__declspec(dllexport) bool DXILoader( dxi::core::IGame * game, const qxml::Document * document )
{
	game->GetInputManager()->AddInputSource( dxi::input::IInputSource::ptr( new dxikeyboard::Keyboard( game ), DeleterKeyboard ) );

	return true;
}

