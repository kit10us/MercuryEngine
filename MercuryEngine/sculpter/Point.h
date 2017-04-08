// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/Matrix.h>

namespace sculpter
{
	class Point
	{
	public:
		Point();
		Point( unify::Matrix matrix );
		~Point();

		unify::Matrix GetMatrix() const;

		const unify::Matrix & operator()() const;

	private:
		unify::Matrix m_matrix;
	};

	struct NamedPoint
	{
		std::string name;
		Point point;
	};
}