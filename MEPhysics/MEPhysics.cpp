// MELuaDLL.cpp : Defines the exported functions for the DLL application.
//

#include "MEPhysics.h"
#include <mephy/GameComponent.h>
#include <memory.h>

void Deleter( mephy::GameComponent * component )
{
	delete component;
}

extern "C" MEPHYSICS_API bool MELoader( me::game::IGame * gameInstance, const qxml::Element * element);

MEPHYSICS_API bool MELoader( me::game::IGame * gameInstance, const qxml::Element * element )
{
	// GameComponent
	mephy::GameComponent * component = new mephy::GameComponent();
	gameInstance->AddComponent( me::game::IGameComponent::ptr( component, Deleter ) );
				  
	return true;
}

