// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

namespace me
{
	namespace scene2d
	{
		enum class Anchor
		{
			Free,
			TopLeft, Top, TopRight, StretchTop,
			Left, Center, Right, StretchLeftRight,
			BottomLeft, Bottom, BottomRight, StretchBottom,
			StretchLeft, StretchTopBottom, StretchFull
		};
	}
}