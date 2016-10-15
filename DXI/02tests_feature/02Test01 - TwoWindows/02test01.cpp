// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/win/DXILib.h>
#include <DXIWinMain.h>

using namespace dxi;

class MyGame : public dxi::core::Game
{
public:
	MyGame()
		: Game( "setup_twowins.xml" )
	{	
	}

	bool Setup( core::IOS * os ) override
	{				  
		return true;
	}
} game;

RegisterGame( game );

