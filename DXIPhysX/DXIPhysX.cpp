// DXILuaDLL.cpp : Defines the exported functions for the DLL application.
//

#include "DXIPhysX.h"
#include <dxi/win/DXILib.h>
#include <dxi/win/DXDevice.h>
#include <dxi/win/DXRenderer.h>
//#include <dxiphysx/Gamepad.h>

/*
void DeleterGamepad( dxigp::Gamepad * gamepad )
{
	delete gamepad;
}
*/

__declspec(dllexport) bool DXILoader( dxi::core::Game * game, const qxml::Document * document )
{
	dxi::win::DXRenderer * dxRenderer = (dxi::win::DXRenderer *)(void*)game->GetOS()->GetRenderer( 0 );
	dxi::win::DX::SetDxDevice( dxRenderer->GetDxDevice() );

	//game->GetInputManager()->AddInputSource( dxi::input::IInputSource::ptr( new dxigp::Gamepad( game ), DeleterGamepad ) );

	return true;
}

