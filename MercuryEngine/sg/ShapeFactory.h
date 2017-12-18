// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved
#pragma once
#include <sg/ShapeCreators.h>
#include <rm/ResourceManagerSimple.h>
#include <me/game/IGame.h>

namespace sg
{
	class ShapeFactory : public rm::ISourceFactory< me::render::Geometry >
	{
	public:
		ShapeFactory( me::game::IGame * gameInstance );
		me::render::Geometry::ptr Produce( unify::Path source, void * data ) override;

	private:
		me::game::IGame * m_game;
	};
}