// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <MEWinMain.h>
#include <me/Game.h>
#include <MainScene.h>

using namespace me;

class MainSceneFactory : public me::scene::ISceneFactory
{
public:
	me::scene::IScene::ptr Produce(me::Game * game)
	{
		return me::scene::IScene::ptr(new MainScene(game));
	}

	std::string GetName() const
	{
		return "Main";
	}
};

class MyGame : public Game
{
public:
	MyGame()
		: Game(me::scene::ISceneFactory::ptr(new MainSceneFactory()), "setup_physx.xml")
	{
	}
} game;

RegisterGame(game);
