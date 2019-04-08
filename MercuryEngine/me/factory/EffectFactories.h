// Copyright (c) 2002 - 2018, Kit10 Studios LLC
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
		typedef std::shared_ptr< rm::ISourceFactory< me::render::Effect > > ptr;

		EffectFactory( game::IGame * gameInstance );
		render::Effect::ptr Produce( unify::Path source, unify::Parameters parameters ) override;
		render::Effect::ptr Produce( unify::Parameters parameters ) override;

	private:
		game::IGame * m_game;
	};
}