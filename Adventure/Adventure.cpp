// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <me/Game.h>
#include <MEWinMain.h>

using namespace me;

class Adventure : public me::Game
{
public:
	Adventure()
		: Game( "setup_testinputsourcemotivator.xml" )
	{
	}

	void Startup() override;
	void Update( UpdateParams params ) override;
} game;

RegisterGame( game );


void Adventure::Startup()
{
}

void Adventure::Update( UpdateParams params )
{
}