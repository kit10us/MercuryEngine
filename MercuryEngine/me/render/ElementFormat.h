// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/V2.h>
#include <unify/V3.h>
#include <unify/V4.h>
#include <unify/TexCoords.h>
#include <unify/Color.h>
#include <string>

namespace me
{
	namespace render
	{
		struct ElementFormat
		{
			enum TYPE
			{
				Float1,
				Float2,
				Float3,
				Float4,
				Matrix4x4,
				ColorUNorm,
				Unknown
			};

			static size_t SizeOf( TYPE format );
			static TYPE FromString( std::string format );
			static std::string ToString( TYPE format );
			static bool Convert( TYPE formatOut, void * out, TYPE formatIn, const void * in );
		};
	}
}