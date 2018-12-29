// MELuaDLL.cpp : Defines the exported functions for the DLL application.
//

#include "MEPhysics.h"
#include <mephy/phy3d/GameComponent3D.h>
#include <memory.h>

void Deleter3D( mephy::phy3d::GameComponent * component )
{
	delete component;
}

extern "C" MEPHYSICS_API bool MELoader( me::game::IGame * gameInstance, const qxml::Element * element);

MEPHYSICS_API bool MELoader( me::game::IGame * gameInstance, const qxml::Element * element )
{
	mephy::phy3d::GameComponent * component = new mephy::phy3d::GameComponent();
	gameInstance->AddComponent( me::game::component::IGameComponent::ptr( component, Deleter3D ) );

	return true;
}

