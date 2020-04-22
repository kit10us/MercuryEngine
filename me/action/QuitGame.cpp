// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/action/QuitGame.h>

using namespace me::action;

QuitGame::QuitGame( game::IGame * gameInstance )
	: m_game{ gameInstance }
{
}

bool QuitGame::Perform()
{
	m_game->Quit();
	return true;
}
