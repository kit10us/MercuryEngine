// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/win/DXILib.h>

class MyGame : public dxi::core::Game
{
public:
	MyGame()
		: Game( "setup_luago.xml" )
	{
	}
} game;

