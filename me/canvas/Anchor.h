// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

namespace me::canvas
{
	/// <summary>
	/// Anchor points for canvas. 
	/// </summary>
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