// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <sculpter/Polygon.h>
#include <unify/String.h>
#include <vector>
#include <map>

namespace sculpter
{
	class PolygonTable
	{
	public:
		PolygonTable();
		~PolygonTable();

	private:
		std::vector< Point > m_points;
		std::map< std::string, size_t, unify::CaseInsensitiveLessThanTest > m_namedPoints;
	};
}