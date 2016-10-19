// DXILuaDLL.cpp : Defines the exported functions for the DLL application.
//

#include "DXIPhysX.h"
#include <dxi/win/DXILib.h>
#include <dxiphysx/SceneManagerComponent.h>

#pragma comment( lib, "PhysX3DEBUG_x86.lib" )
#pragma comment( lib, "PhysX3CommonDEBUG_x86.lib" )
#pragma comment( lib, "PhysXProfileSDKDEBUG.lib" )
#pragma comment( lib, "PhysX3CharacterKinematicDEBUG_x86.lib" )
#pragma comment( lib, "PhysX3CookingDEBUG_x86.lib" )
#pragma comment( lib, "PxTaskDEBUG.lib" )
#pragma comment( lib, "PhysX3ExtensionsDEBUG.lib" )

void DeleterSMC( dxiphysx::SceneManagerComponent * smc )
{
	delete smc;
}

__declspec(dllexport) bool DXILoader( dxi::core::Game * game, const qxml::Document * document )
{
	game->GetSceneManager()->AddComponent( dxi::scene::ISceneManagerComponent::ptr( new dxiphysx::SceneManagerComponent( game->GetOS() ), DeleterSMC ) );
	return true;
}

