// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/action/IAction.h>
#include <me/IGame.h>
#include <string>

namespace me
{
	namespace action
	{
		/// <summary>
		/// Writes a message to the log. Used for debugging.
		/// </summary>
		class LogMessage : public IAction
		{
		public:
			LogMessage( IGame * game, std::string message );

		public: // IAction...
			bool Perform() override;

		private:
			IGame * m_game;
			std::string m_message;
		};
	}
}
