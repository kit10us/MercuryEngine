// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <MEWinMain.h>
#include <me/game/Game.h>
#include <MainScene.h>

using namespace me;

#ifdef _DEBUG
#pragma comment( lib, "../../../extensions/Debug/MEPhysics.lib" )
#else
#pragma comment( lib, "../../../extensions/Release/MEPhysics.lib" )
#endif

class MainSceneFactory : public me::scene::ISceneFactory
{
public:
	me::scene::IScene::ptr Produce( me::game::Game * gameInstance )
	{
		return me::scene::IScene::ptr( new MainScene( gameInstance ) );
	}

	std::string GetName() const
	{
		return "Main";
	}
};

class MyGame : public game::Game
{
public:
	MyGame()
		: Game(me::scene::ISceneFactory::ptr(new MainSceneFactory()), unify::Path( "setup_mephysics2d.xml" ) )
	{
	}
} myGame;

RegisterGame( myGame );
