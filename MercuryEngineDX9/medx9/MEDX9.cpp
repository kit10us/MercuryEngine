// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <medx9/WindowsOS.h>
#include <memory.h>
//#include <dxi/win/DXILib.h>
//#include <dxi/win/DXRenderer.h>
#include <me/game/IGame.h>

using namespace medx9;

void Deleter( WindowsOS * factory )
{
	delete factory;
}

extern "C" __declspec(dllexport) bool MELoader( me::game::IGame * gameInstance, const qxml::Element * element );

__declspec(dllexport) bool MELoader( me::game::IGame * gameInstance, const qxml::Element * element )
{
	gameInstance->SetOS( me::IOS::ptr( new WindowsOS( gameInstance, element ) ) );

	return true;
}

