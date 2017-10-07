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
		/// Restarts the current scene.
		/// </summary>
		class RestartScene : public IAction
		{
		public:
			RestartScene( IGame * game );

		public: // IAction...
			bool Perform() override;

		private:
			IGame * m_game;
		};
	}
}
