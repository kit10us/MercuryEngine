// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <MEWinMain.h>
#include <me/game/Game.h>
#include <MainScene.h>

using namespace me;

#ifdef _DEBUG
#pragma comment( lib, "../../../../extensions/Debug/MEPhysics.lib" )
#else
#pragma comment( lib, "../../../../extensions/Release/MEPhysics.lib" )
#endif

class MyGame : public game::Game
{
public:
	MyGame()
		: Game( "Main", unify::Path( "setup_mephysics2d.xml" ) )
	{
	}

	void AddScenes( scene::SceneManager * sceneManager ) override
	{
		sceneManager->AddScene( "Main", me::scene::IScene::ptr( new MainScene( this ) ) );
	}
} myGame;

RegisterGame( myGame );
