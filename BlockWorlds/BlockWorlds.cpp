// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <MEWinMain.h>
#include <me/game/Game.h>
#include <MainScene.h>

using namespace me;

class MainSceneFactory : public me::scene::ISceneFactory
{
public:
	me::scene::IScene::ptr Produce( me::game::Game * gameInstance )
	{
		return me::scene::IScene::ptr(new MainScene( gameInstance ));
	}

	std::string GetName() const
	{
		return "Main";
	}
};

class BlockWorlds : public game::Game
{
public:
	BlockWorlds() 
		: Game( me::scene::ISceneFactory::ptr(new MainSceneFactory()), unify::Path( "setup.xml" ) ) 
	{
	}

} myGame;

RegisterGame( myGame );
