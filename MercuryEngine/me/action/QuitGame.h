// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/action/IAction.h>
#include <me/IGame.h>

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
			QuitGame( IGame * game );

		public: // IAction...
			bool Perform() override;

		private:
			IGame * m_game;
		};
	}
}
