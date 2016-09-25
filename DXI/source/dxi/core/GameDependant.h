// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/core/IGame.h>

namespace dxi
{
	namespace core
	{
		class GameDependant
		{
		public:
			GameDependant( dxi::core::IGame * game );
			virtual ~GameDependant();

		protected:
			IGame * Game();

		private:
			IGame * m_game;
		};
	}
}