// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once
#include <dxi/shapes/ShapeCreators.h>
#include <rm/ResourceManagerSimple.h>

namespace dxi
{
	class ShapeJsonFactory : public rm::IJsonFactory< Geometry >
	{
	public:
		Geometry * Produce( qjson::Object json ) override;
	};

	class ShapeXMLFactory : public rm::IXMLFactory< Geometry >
	{
	public:
		Geometry * Produce( const qxml::Element & element ) override;
	};
}