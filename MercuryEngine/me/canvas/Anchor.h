// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

namespace me
{
	namespace canvas
	{
		enum class Anchor
		{
			Free,
			TopLeft, Top, TopRight, StretchTop,
			Left, Center, Right, StretchLeftRight,
			BottomLeft, Bottom, BottomRight, StretchBottom,
			StretchLeft, StretchTopBottom, StretchRight, 
			StretchFull
		};
	}
}