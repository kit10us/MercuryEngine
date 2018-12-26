// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/game/Game.h>
#include <me/setup/SetupScript.h>

using namespace me;
using namespace setup;

SetupScript::SetupScript( game::Game * game, unify::Path path )
	: m_game{ game }
	, m_path{ path }
{
}

SetupScript::~SetupScript()
{
	m_game = nullptr;
}

game::Game * SetupScript::GetGame()
{
	return m_game;
}

bool SetupScript::Reload()
{
	// We don't load scripts directly.
	return true;
}

std::string SetupScript::GetSource() const
{
	return m_path.ToString();
}
