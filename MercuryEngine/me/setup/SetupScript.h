// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/game/Game.h>
#include <me/script/IScript.h>

namespace me
{
	namespace setup
	{
		/// <summary>
		/// A setup script.
		/// </summary>
		class SetupScript : public me::script::IScript
		{
			game::Game * m_game;
			unify::Path m_path;
		public:
			SetupScript( game::Game * game, unify::Path path );
			~SetupScript();

			game::Game * GetGame();

		public: // IResource...
			bool Reload() override;
			std::string GetSource() const override;
		};
	}
}