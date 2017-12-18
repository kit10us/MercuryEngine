// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <me/render/Mesh.h>
#include <me/game/IGame.h>
#include <rm/ResourceManagerSimple.h>

namespace me
{
	class GeometryFactory : public rm::ISourceFactory< render::Geometry >
	{
	public:
		GeometryFactory( game::IGame * gameInstance );
		render::Geometry::ptr Produce( unify::Path source, void * data ) override;

	private:
		game::IGame * m_game;
	};
}