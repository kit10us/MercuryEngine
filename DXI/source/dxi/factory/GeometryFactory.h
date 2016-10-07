// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/Mesh.h>
#include <rm/ResourceManagerSimple.h>
#include <dxi/core/Game.h>

namespace dxi
{
	class GeometryFactory : public rm::ISourceFactory< Geometry >
	{
	public:
		GeometryFactory( core::Game * game );
		 Geometry * Produce( unify::Path source ) override;

	private:
		core::Game * m_game;
	};
}