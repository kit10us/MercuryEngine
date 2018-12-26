// Copyright (c) 2002 - 2018, Evil Quail LLC
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
			typedef std::shared_ptr< rm::ISourceFactory< me::script::IScript > > ptr;

			SetupScriptFactory( game::Game * game );
			script::IScript::ptr Produce( unify::Path source, void * data ) override;

		private:
			game::Game * m_game;
		};
	}
}
