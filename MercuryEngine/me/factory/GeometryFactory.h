// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <me/Mesh.h>
#include <me/IGame.h>
#include <rm/ResourceManagerSimple.h>

namespace me
{
	class GeometryFactory : public rm::ISourceFactory< Geometry >
	{
	public:
		GeometryFactory( IGame * game );
		Geometry::ptr Produce( unify::Path source, void * data ) override;

	private:
		IGame * m_game;
	};
}