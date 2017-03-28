// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <MEWinMain.h>
#include <me/Game.h>

using namespace me;

class MyGame : public Game
{
public:
	MyGame()
		: Game( unify::Path( "setup_testinputsourcemotivator.xml" ) ) {}
} game;

RegisterGame(game);
