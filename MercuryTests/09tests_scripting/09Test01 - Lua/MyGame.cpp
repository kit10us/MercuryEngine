// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

///<summary>
/// Load lua extension.
///</summary>

#include <MEWinMain.h>
#include <me/game/Game.h>
#include <me/scene/DefaultSceneFactory.h>

using namespace me;

class MyGame : public game::Game
{
public:
	MyGame() : Game( unify::Path( "setup_luago.xml" ) ) {}
} myGame;

RegisterGame( myGame );
