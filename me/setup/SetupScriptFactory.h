// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/script/IScript.h>
#include <me/game/Game.h>
#include <rm/ResourceManager.h>

namespace me
{
	namespace setup
	{
		class SetupScriptFactory : public rm::ISourceFactory< me::script::IScript >
		{
		public:
			using ptr = std::shared_ptr< me::script::IScript>;

			SetupScriptFactory( game::Game * game );

			//unify::Result<std::shared_ptr< T >> Produce( unify::Path path, unify::Parameters parameters = {} ) = 0;
			unify::Result<std::shared_ptr<script::IScript>> Produce( unify::Path source, unify::Parameters parameters ) override;
			
			//unify::Result<std::shared_ptr< T >> Produce( unify::Parameters parameters ) = 0;
			unify::Result<std::shared_ptr<script::IScript>> Produce( unify::Parameters parameters ) override;

		private:
			game::Game * m_game;
		};
	}
}
