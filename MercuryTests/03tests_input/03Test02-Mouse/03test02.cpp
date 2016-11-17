// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <me/Game.h>
#include <MEWinMain.h>

class MyGame : public me::Game
{
public:
	MyGame()
		: Game( "setup_testmouse.xml" )
	{
	}
} game;

RegisterGame( game );

