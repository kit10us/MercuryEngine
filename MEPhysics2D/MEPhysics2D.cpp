// MELuaDLL.cpp : Defines the exported functions for the DLL application.
//

#include "MEPhysics2D.h"
#include <mephy2d/GameComponent.h>
#include <memory.h>

void Deleter( mephy2d::GameComponent * component )
{
	delete component;
}

extern "C" MEPHYSICS2D_API bool MELoader( me::game::IGame * gameInstance, const qxml::Element * element);

MEPHYSICS2D_API bool MELoader( me::game::IGame * gameInstance, const qxml::Element * element )
{
	// GameComponent
	mephy2d::GameComponent * component = new mephy::GameComponent();
	gameInstance->AddComponent( me::game::IGameComponent::ptr( component, Deleter ) );
				  
	return true;
}

