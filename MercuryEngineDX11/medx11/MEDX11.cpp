// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <medx11/WindowsOS.h>
#include <memory.h>
#include <me/IGame.h>

using namespace medx11;

void Deleter( WindowsOS * factory )
{
	delete factory;
}

extern "C" __declspec(dllexport) bool MELoader( me::IGame * game, const qxml::Element * element );

__declspec(dllexport) bool MELoader( me::IGame * game, const qxml::Element * element )
{
	game->SetOS( me::IOS::ptr( new WindowsOS( game, element ) ) );

	return true;
}

