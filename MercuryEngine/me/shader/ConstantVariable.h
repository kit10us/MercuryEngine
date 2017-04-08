// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/render/ElementFormat.h>
#include <string>
#include <vector>

namespace me
{
	namespace shader
	{
		struct ConstantVariable
		{
			ConstantVariable();
			ConstantVariable( std::string name, render::ElementFormat::TYPE type, size_t count );

			std::string name;
			render::ElementFormat::TYPE type;
			std::vector< float > default;
			bool hasDefault;
			size_t count;
			size_t offsetInBytes;
		};
	}
}