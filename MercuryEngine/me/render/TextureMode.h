// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/String.h>

namespace me
{
	namespace render
	{
		struct TextureMode
		{
			enum TYPE
			{
				Correct,	// Repeats the same texture on all sides, identically
				Wrapped		// Wraps the texture around the object so it uses only a portion on each side
			};

			static TYPE FromString( std::string mode );
		};
	}
}