// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/action/IAction.h>
#include <me/game/IGame.h>

namespace me::action
{
	/// <summary>
	/// Restarts the current scene.
	/// </summary>
	class RestartScene : public IAction
	{
		game::IGame* m_game;

	public:
		RestartScene( game::IGame * gameInstance );

	public: // IAction...
		bool Perform() override;
	};
}