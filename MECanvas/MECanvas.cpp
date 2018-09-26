// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/game/Game.h>
#include <MECanvas.h>
#include <canvas/SceneComponent.h>

void DeleterSceneComponent( canvas::SceneComponent * component )
{
	delete component;
}

extern "C" MECANVAS_API bool MELoader( me::game::IGame * gameInstance, const qxml::Element * element);

MECANVAS_API bool MELoader( me::game::IGame * gameInstance, const qxml::Element * element )
{
	// Create the null extension game component.
	canvas::SceneComponent * sceneComponent = new canvas::SceneComponent( gameInstance );

	// Add game component to our game instance. 
	// Requires a custom deleter which, because it's local to the DLL will used the DLL's memory block.
	gameInstance->AddComponent( me::game::IGameComponent::ptr( sceneComponent, DeleterSceneComponent ) );

	return true;
}

