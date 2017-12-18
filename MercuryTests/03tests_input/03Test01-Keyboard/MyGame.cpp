// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <MEWinMain.h>
#include <me/game/Game.h>
#include <me/scene/DefaultSceneFactory.h>

using namespace me;

class MyGame : public game::Game
{
public:
	MyGame() : Game( unify::Path( "setup_testkeyboard.xml" ) ) {}
} myGame;

RegisterGame( myGame );
