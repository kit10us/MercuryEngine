// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/Matrix.h>

namespace sculpter
{
	class Point
	{
	public:
		Point( unify::Matrix matrix );
		~Point();

	private:
		unify::Matrix m_matrix;
	};

	struct NamedPoint
	{
		std::string name;
		Point point;
	};
}