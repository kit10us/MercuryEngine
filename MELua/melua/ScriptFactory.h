// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <melua/ScriptEngine.h>
#include <me/script/IScript.h>
#include <me/game/IGame.h>
#include <rm/ResourceManagerSimple.h>

namespace melua
{
	class ScriptFactory : public rm::ISourceFactory< me::script::IScript >
	{
	public:
		ScriptFactory( ScriptEngine * se );
		me::script::IScript::ptr Produce( unify::Path source, void * data ) override;

	private:
		ScriptEngine * m_se;
	};
}
