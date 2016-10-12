// DXILuaDLL.cpp : Defines the exported functions for the DLL application.
//

#include "DXIKeyboard.h"
#include <dxi/win/DXILib.h>
#include <dxi/win/DXDevice.h>
#include <dxi/win/DXRenderer.h>
#include <dxikeyboard/Keyboard.h>

void DeleterKeyboard( dxikeyboard::Keyboard * keyboard )
{
	delete keyboard;
}

__declspec(dllexport) bool DXILoader( dxi::core::Game * game, const qxml::Document * document )
{
	dxi::win::DXRenderer * dxRenderer = (dxi::win::DXRenderer *)(void*)game->GetOS().GetRenderer( 0 );
	dxi::win::DX::SetDxDevice( dxRenderer->GetDxDevice() );

	game->GetInputManager()->AddInputSource( dxi::input::IInputSource::ptr( new dxikeyboard::Keyboard( game ), DeleterKeyboard ) );

	return true;
}

