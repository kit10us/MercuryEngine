// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <mewos/WindowsOS.h>
#include <medx9/RendererFactory.h>
#include <memory.h>
#include <me/game/IGame.h>

void Deleter( mewos::WindowsOS * factory )
{
	delete factory;
}

extern "C" __declspec(dllexport) bool MELoader( me::game::IGame * gameInstance, const qxml::Element * element );

__declspec(dllexport) bool MELoader( me::game::IGame * gameInstance, const qxml::Element * element )
{
	gameInstance->SetOS( 
		me::IOS::ptr( 
			new mewos::WindowsOS( gameInstance, element, me::render::IRendererFactory::ptr{ new medx9::RendererFactory } ) 
		) 
	);

	return true;
}

