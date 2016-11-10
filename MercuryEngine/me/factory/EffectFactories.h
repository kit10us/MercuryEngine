// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/Effect.h>
#include <me/IGame.h>
#include <rm/ResourceManagerSimple.h>

namespace me
{
	class EffectFactory : public rm::ISourceFactory< me::Effect >
	{
	public:
		EffectFactory( IGame * game );
		Effect::ptr Produce( unify::Path source, void * data ) override;

	private:
		IGame * m_game;
	};
}