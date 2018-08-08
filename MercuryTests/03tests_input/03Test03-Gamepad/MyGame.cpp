// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <MEWinMain.h>
#include <me/game/Game.h>
#include <me/scene/DefaultSceneFactory.h>

using namespace me;

class MyGame : public game::Game
{
public:
	MyGame()
		: Game(me::scene::ISceneFactory::ptr(new  me::scene::DefaultSceneFactory("Main")), unify::Path( "setup_testgamepad.xml" ) )
	{
	}
} myGame;

RegisterGame( myGame );
