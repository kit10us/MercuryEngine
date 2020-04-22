// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/action/IAction.h>
#include <me/game/IGame.h>

namespace me::action
{
	/// <summary>
	/// Quits out of the game.
	/// </summary>
	class QuitGame : public IAction
	{
		game::IGame* m_game;

	public:
		QuitGame( game::IGame * gameInstance );

	public: // IAction...
		bool Perform() override;

	};
}