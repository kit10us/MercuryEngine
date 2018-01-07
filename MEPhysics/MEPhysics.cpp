// MELuaDLL.cpp : Defines the exported functions for the DLL application.
//

#include "MEPhysics.h"
#include <mephy/phy3d/GameComponent3D.h>
#include <mephy/phy2d/GameComponent2D.h>
#include <memory.h>

void Deleter3D( mephy::phy3d::GameComponent * component )
{
	delete component;
}

void Deleter2D( mephy::phy2d::GameComponent * component )
{
	delete component;
}

extern "C" MEPHYSICS_API bool MELoader( me::game::IGame * gameInstance, const qxml::Element * element);

MEPHYSICS_API bool MELoader( me::game::IGame * gameInstance, const qxml::Element * element )
{
	// GameComponents

	{
		mephy::phy3d::GameComponent * component = new mephy::phy3d::GameComponent();
		gameInstance->AddComponent( me::game::IGameComponent::ptr( component, Deleter3D ) );
	}
	
	{
		mephy::phy2d::GameComponent * component = new mephy::phy2d::GameComponent();
		gameInstance->AddComponent( me::game::IGameComponent::ptr( component, Deleter2D ) );
	}

	return true;
}

