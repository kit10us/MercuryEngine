// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/render/Effect.h>
#include <me/game/IGame.h>
#include <rm/ResourceManager.h>

namespace me
{
	class EffectFactory : public rm::ISourceFactory< render::Effect >
	{
	public:
		EffectFactory( game::IGame * gameInstance );
		render::Effect::ptr Produce( unify::Path source, void * data ) override;

	private:
		game::IGame * m_game;
	};
}