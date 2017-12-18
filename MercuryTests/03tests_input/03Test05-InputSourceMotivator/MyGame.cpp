// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <MEWinMain.h>
#include <me/game/Game.h>

using namespace me;

class MyGame : public game::Game
{
public:
	MyGame()
		: Game( unify::Path( "setup_testinputsourcemotivator.xml" ) ) {}
} myGame;

RegisterGame( myGame );
