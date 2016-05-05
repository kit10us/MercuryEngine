// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/geo/Geometry.h>
#include <dxi/ResourceManager.h>

namespace dxi
{
	class GeometryFactory : public ResourceManager< geo::Geometry >::IFactory
	{
	public:
		geo::Geometry * Produce( const qjson::Object & object );
	};
}