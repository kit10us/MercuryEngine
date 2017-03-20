// Copyright (c) 2003 - 2017, Quentin S. Smith
// All Rights Reserved

#include <me/action/QuitGame.h>

using namespace me;
using namespace action;

QuitGame::QuitGame( IGame * game )
	: m_game{ game }
{
}

bool QuitGame::Perform()
{
	m_game->Quit();
	return true;
}
