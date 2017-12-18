// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/action/IAction.h>
#include <me/game/IGame.h>
#include <string>

namespace me
{
	namespace action
	{
		/// <summary>
		/// Send a command through the game.
		/// </summary>
		class SendCommand : public IAction
		{
		public:
			SendCommand( game::IGame * gameInstance, size_t id, std::string extra );

		public: // IAction...
			bool Perform() override;

		private:
			game::IGame * m_game;
			size_t m_id;
			std::string m_extra;
		};
	}
}
