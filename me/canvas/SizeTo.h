// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

namespace me::canvas
{
	struct SizeTo
	{
		enum class Type {
			Pixels,
			Percent,
			Weighted
		};

		Type type;
		float value;

		SizeTo( Type type, float value );

		float GetSize( float parentSize, size_t totalWeight ) const;
	};
}

	