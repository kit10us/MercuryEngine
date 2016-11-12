// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <me/Game.h>
#include <dxi/win/DXILib.h>
#include <DXIWinMain.h>

using namespace me;

class MyGame : public Game
{
public:
	MyGame()
		: Game( "setup_twowins.xml" )
	{	
	}

	bool Setup( me::IOS * os ) override
	{				  
		return true;
	}
} game;

RegisterGame( game );

