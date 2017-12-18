// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <medx11/WindowsOS.h>
#include <memory.h>
#include <me/game/IGame.h>

using namespace medx11;

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

