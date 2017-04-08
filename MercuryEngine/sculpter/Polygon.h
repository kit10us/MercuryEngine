// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <sculpter/PointTable.h>
#include <memory>

namespace sculpter
{
	class Polygon
	{
	public:
		typedef std::shared_ptr< Polygon > ptr;

		Polygon( Point origin );
		~Polygon();

	private:
		PointTable m_pointTable;
	};
}