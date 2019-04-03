// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved
#pragma once

#include <me/render/Mesh.h>
#include <me/game/IGame.h>
#include <rm/ResourceManager.h>

namespace me
{
	class GeometryFactory : public rm::ISourceFactory< render::Geometry >
	{
	public:
		typedef std::shared_ptr< rm::ISourceFactory< me::render::Geometry > > ptr;

		GeometryFactory( game::IGame * gameInstance );
		render::Geometry::ptr Produce( unify::Path source, void * data ) override;
		render::Geometry::ptr Produce( void * data ) override;
		render::Geometry::ptr Produce( unify::Parameters parameters ) override;

	private:
		game::IGame * m_game;
	};
}