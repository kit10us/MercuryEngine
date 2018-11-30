// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/script/IScript.h>
#include <me/game/IGame.h>
#include <rm/ResourceManagerSimple.h>

namespace me
{
	namespace script
	{
		class ScriptFactory : public rm::ISourceFactory< script::IScript >
		{
		public:
			ScriptFactory( game::IGame * gameInstance );
			IScript::ptr Produce( unify::Path source, void * data ) override;

		private:
			game::IGame * m_game;
		};
	}
}