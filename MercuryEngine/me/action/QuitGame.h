// Copyright (c) 2003 - 2017, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/action/IAction.h>
#include <me/IGame.h>

namespace me
{
	namespace action
	{
		/// <summary>
		/// Ignores an actions return result (forces a true result).
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
