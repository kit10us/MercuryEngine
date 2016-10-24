// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/Effect.h>
#include <rm/ResourceManagerSimple.h>
#include <dxi/core/IGame.h>

namespace dxi
{
	class EffectFactory : public rm::ISourceFactory< Effect >
	{
	public:
		EffectFactory( core::IGame * game );
		std::shared_ptr< Effect > Produce( unify::Path source ) override;

	private:
		core::IGame * m_game;
	};
}