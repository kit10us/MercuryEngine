// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

namespace me::canvas
{
	/// <summary>
	/// Manages the sizing of an element.
	/// </summary>
	struct SizeTo
	{
		/// <summary>
		/// Specifies the sizing method of an element.
		/// </summary>
		enum class Type {
			Pixels,
			Percent,
			Weighted
		};

		Type type;
		float value;

		SizeTo( Type type, float value );

		/// <summary>
		/// Returns the size modified by the sizing method.
		/// </summary>
		/// <returns></returns>
		float GetSize( float parentSize, size_t totalWeight ) const;
	};
}

	