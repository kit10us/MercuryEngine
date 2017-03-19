// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <MEWinMain.h>
#include <me/Game.h>
#include <MainScene.h>

using namespace me;

class BlockWorlds : public Game
{
public:
	BlockWorlds() : Game( "setup.xml" ) {}

	scene::IScene::ptr BlockWorlds::CreateMainScene();
} game;

RegisterGame( game );

scene::IScene::ptr BlockWorlds::CreateMainScene()
{
	return scene::IScene::ptr( new MainScene( this ) );
}
