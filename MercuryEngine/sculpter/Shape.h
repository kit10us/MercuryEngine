// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <sculpter/PolygonTable.h>
#include <memory>

namespace sculpter
{
	class Shape
	{
	public:
		typedef std::shared_ptr< Shape > ptr;

		Shape();
		~Shape();

	private:
		PolygonTable m_polygons;
	};
}