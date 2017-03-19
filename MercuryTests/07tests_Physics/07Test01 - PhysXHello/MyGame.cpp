// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <MEWinMain.h>
#include <me/Game.h>
#include <MainScene.h>

using namespace me;

class MyGame : public Game
{
public:
	MyGame()
		: Game( "setup_physx.xml" )
	{
	}

	scene::IScene::ptr MyGame::CreateMainScene();
} game;

RegisterGame( game );

scene::IScene::ptr MyGame::CreateMainScene()
{
	return scene::IScene::ptr( new MainScene( this ) );
}
