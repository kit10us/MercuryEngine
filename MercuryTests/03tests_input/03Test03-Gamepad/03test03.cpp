// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <me/Game.h>
#include <dxi/win/DXILib.h>
#include <MEWinMain.h>

class MyGame : public me::Game
{
public:
	MyGame()
		: Game( "setup_testgamepad.xml" )
	{
	}
} game;

RegisterGame( game );

