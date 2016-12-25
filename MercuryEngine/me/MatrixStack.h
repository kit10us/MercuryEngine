// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/Matrix.h>
#include <vector>
#include <map>

namespace me
{
	class MatrixStack
	{
	public:
		MatrixStack();

		size_t AddMatrix( std::string name, unify::Matrix matrix );

	private:
		std::vector< unify::Matrix > m_matrices;
		std::map< std::string, size_t > m_map;
		std::vector< size_t > m_parent;
	};
}