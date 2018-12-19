// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <MEWinMain.h>
#include <me/game/Game.h>
#include <MainScene.h>

using namespace me;

class Adventure : public game::Game
{
public:
	Adventure()
		: Game( "Main", unify::Path( "setup.xml" ) )
	{
	}

	void AddScenes( scene::SceneManager * sceneManager ) override
	{
		sceneManager->AddScene( "Main", me::scene::IScene::ptr( new MainScene( this ) ) );
	}
} myGame;

RegisterGame( myGame );
