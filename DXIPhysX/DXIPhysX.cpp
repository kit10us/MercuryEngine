// DXILuaDLL.cpp : Defines the exported functions for the DLL application.
//

#include "DXIPhysX.h"
#include <dxi/win/DXILib.h>
#include <dxiphysx/GameComponent.h>
					   
#pragma comment( lib, "PhysX3DEBUG_x86.lib" )
#pragma comment( lib, "PhysX3CommonDEBUG_x86.lib" )
#pragma comment( lib, "PhysXProfileSDKDEBUG.lib" )
#pragma comment( lib, "PhysX3CharacterKinematicDEBUG_x86.lib" )
#pragma comment( lib, "PhysX3CookingDEBUG_x86.lib" )
#pragma comment( lib, "PxTaskDEBUG.lib" )
#pragma comment( lib, "PhysX3ExtensionsDEBUG.lib" )

void Deleter( dxiphysx::GameComponent * component )
{
	delete component;
}

DXIPHYSX_API bool DXILoader( dxi::core::Game * game, const qxml::Document * document )
{
	game->AddComponent( dxi::core::IGameComponent::ptr( new dxiphysx::GameComponent(), Deleter ) );
	return true;
}

