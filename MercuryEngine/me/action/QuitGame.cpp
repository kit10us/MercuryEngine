// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <me/action/QuitGame.h>

using namespace me;
using namespace action;

QuitGame::QuitGame( game::IGame * gameInstance )
	: m_game{ gameInstance }
{
}

bool QuitGame::Perform()
{
	m_game->Quit();
	return true;
}
