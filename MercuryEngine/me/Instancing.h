// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <string>

namespace me
{
	struct Instancing
	{
	private:
		Instancing();
	public:

		enum TYPE
		{
			None,
			QP,
			Matrix
		};

        static std::string ToString( TYPE value );
        static TYPE FromString( std::string value );
	};
}
