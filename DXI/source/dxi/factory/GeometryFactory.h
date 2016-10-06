// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/Mesh.h>
#include <rm/ResourceManagerSimple.h>

namespace dxi
{
	class GeometryFactory : public rm::ISourceFactory< Geometry >
	{
	public:
		 Geometry * Produce( unify::Path source ) override;
	};
}