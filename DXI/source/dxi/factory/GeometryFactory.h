// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/Mesh.h>
#include <rm/ResourceManagerSimple.h>
#include <dxi/core/IGame.h>

namespace dxi
{
	class GeometryFactory : public rm::ISourceFactory< Geometry >
	{
	public:
		GeometryFactory( core::IGame * game );
		 Geometry::ptr Produce( unify::Path source, void * data ) override;

	private:
		core::IGame * m_game;
	};
}