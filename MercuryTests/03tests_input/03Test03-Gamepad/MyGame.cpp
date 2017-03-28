// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <MEWinMain.h>
#include <me/Game.h>
#include <me/scene/DefaultSceneFactory.h>

using namespace me;

class MyGame : public Game
{
public:
	MyGame()
		: Game(me::scene::ISceneFactory::ptr(new  me::scene::DefaultSceneFactory("Main")), unify::Path( "setup_testgamepad.xml" ) )
	{
	}
} game;

RegisterGame( game );
