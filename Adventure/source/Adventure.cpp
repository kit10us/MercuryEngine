// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <MEWinMain.h>
#include <me/game/Game.h>
#include <MainScene.h>
#include <LoadScene.h>

using namespace me;

class Adventure : public game::Game
{
public:
	Adventure()
		: Game( unify::Path( "setup.xml" ) )
	{
	}

	void AddScenes( scene::SceneManager * sceneManager ) override
	{
		sceneManager->AddScene( me::scene::IScene::ptr( new MainScene( this ) ) );
		sceneManager->AddScene( me::scene::IScene::ptr( new LoadScene( this ) ) );
	}
} myGame;

RegisterGame( myGame );
