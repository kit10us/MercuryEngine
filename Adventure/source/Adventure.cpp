// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <MEWinMain.h>
#include <me/Game.h>
#include <MainScene.h>

using namespace me;

class Adventure : public Game
{
public:
	Adventure()
		: Game( "setup.xml" )
	{
	}

	scene::IScene::ptr CreateMainScene();
} game;

RegisterGame( game );

scene::IScene::ptr Adventure::CreateMainScene()
{
	return scene::IScene::ptr( new MainScene( this ) );
}
