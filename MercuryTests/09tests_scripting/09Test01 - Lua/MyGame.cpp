// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <MEWinMain.h>
#include <me/Game.h>
#include <me/scene/DefaultSceneFactory.h>

using namespace me;

class MyGame : public Game
{
public:
	MyGame() : Game( unify::Path( "setup_luago.xml" ) ) {}
} game;

RegisterGame(game);
