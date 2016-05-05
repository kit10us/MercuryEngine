// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/GeometryFactory.h>
#include <dxi/exception/FailedToCreate.h>

using namespace dxi;

Geometry * GeometryFactory::Produce( unify::Path source )
{
	if ( ! source.Empty() )
	{
		return nullptr; // TODO: mesh loading. Complicate topic.
	}

	throw exception::FailedToCreate( "No valid factory option provided for geometry!" );
}