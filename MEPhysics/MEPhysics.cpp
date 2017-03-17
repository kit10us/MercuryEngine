// DXILuaDLL.cpp : Defines the exported functions for the DLL application.
//

#include "MEPhysics.h"
#include <mephy/GameComponent.h>
#include <memory.h>

void Deleter( mephy::GameComponent * component )
{
	delete component;
}

extern "C" MEPHYSICS_API bool MELoader( me::IGame * game, const qxml::Element * element);

MEPHYSICS_API bool MELoader( me::IGame * game, const qxml::Element * element )
{
	// GameComponent
	mephy::GameComponent * component = new mephy::GameComponent();
	game->AddComponent( me::IGameComponent::ptr( component, Deleter ) );
				  
	return true;
}

