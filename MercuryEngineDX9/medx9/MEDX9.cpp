// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <medx9/WindowsOS.h>
#include <memory.h>
//#include <dxi/win/DXILib.h>
//#include <dxi/win/DXRenderer.h>
#include <me/IGame.h>

using namespace medx9;

void Deleter( WindowsOS * factory )
{
	delete factory;
}

extern "C" __declspec(dllexport) bool DXILoader( me::IGame * game, const qxml::Document * doc );

__declspec(dllexport) bool DXILoader( me::IGame * game, const qxml::Document * doc )
{
	game->SetOS( me::IOS::ptr( new WindowsOS( game ) ) );

	return true;
}

