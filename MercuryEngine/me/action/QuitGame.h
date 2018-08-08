// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/action/IAction.h>
#include <me/game/IGame.h>

namespace me
{
	namespace action
	{
		/// <summary>
		/// Quits out of the game.
		/// </summary>
		class QuitGame : public IAction
		{
		public:
			QuitGame( game::IGame * gameInstance );

		public: // IAction...
			bool Perform() override;

		private:
			game::IGame * m_game;
		};
	}
}
