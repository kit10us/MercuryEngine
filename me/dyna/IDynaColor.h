// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/dyna/Dyna.h>
#include <unify/ColorUnit.h>
#include <unify/TimeDelta.h>
#include <memory>

namespace me::dyna
{
	/// <summary>
	/// Returns a color at a given time.
	/// </summary>
	class IDynaColor
	{
	public:
		typedef std::shared_ptr< IDynaColor > ptr;
		virtual ~IDynaColor() {}

		/// <summary>
		/// Returns the color at a given time.
		/// </summary>
		virtual Result GetColor( unify::ColorUnit& out, unify::TimeDelta delta ) = 0;
	};
}