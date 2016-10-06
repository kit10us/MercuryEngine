// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once
#include <dxi/shapes/ShapeCreators.h>
#include <rm/ResourceManagerSimple.h>

namespace dxi
{
	class ShapeFactory : public rm::ISourceFactory< Geometry >
	{
	public:
		Geometry * Produce( unify::Path source ) override;
	};
}