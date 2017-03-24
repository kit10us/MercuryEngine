// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <sculpter/Point.h>
#include <unify/String.h>
#include <vector>
#include <map>

namespace sculpter
{
	class PointTable
	{
	public:
		PointTable( std::initializer_list< NamedPoint > points );
		PointTable( Point origin );
		~PointTable();

		void AddNamedPoint( std::string name, Point point );
		void AddPoint( NamedPoint point );
		void AddPoint( Point point );

	private:
		std::vector< Point > m_points;
		std::map< std::string, size_t, unify::CaseInsensitiveLessThanTest > m_namedPoints;
	};
}