// DXILuaDLL.cpp : Defines the exported functions for the DLL application.
//

#include "DXIPhysX.h"
#include <dxiphysx/GameComponent.h>
					   
#ifdef _DEBUG
#pragma comment( lib, "PhysX3DEBUG_x86.lib" )
#pragma comment( lib, "PhysX3CommonDEBUG_x86.lib" )
#pragma comment( lib, "PhysXProfileSDKDEBUG.lib" )
#pragma comment( lib, "PhysX3CharacterKinematicDEBUG_x86.lib" )
#pragma comment( lib, "PhysX3CookingDEBUG_x86.lib" )
#pragma comment( lib, "PxTaskDEBUG.lib" )
#pragma comment( lib, "PhysX3ExtensionsDEBUG.lib" )
#else
#pragma comment( lib, "PhysX3_x86.lib" )
#pragma comment( lib, "PhysX3Common_x86.lib" )
#pragma comment( lib, "PhysXProfileSDK.lib" )
#pragma comment( lib, "PhysX3CharacterKinematic_x86.lib" )
#pragma comment( lib, "PhysX3Cooking_x86.lib" )
#pragma comment( lib, "PxTask.lib" )
#pragma comment( lib, "PhysX3Extensions.lib" )
#endif


void Deleter( dxiphysx::GameComponent * component )
{
	delete component;
}

extern "C" DXIPHYSX_API bool DXILoader( me::IGame * game, const qxml::Document * doc );
DXIPHYSX_API bool DXILoader( me::IGame * game, const qxml::Document * document )
{
	game->AddComponent( me::IGameComponent::ptr( new dxiphysx::GameComponent(), Deleter ) );
	return true;
}

