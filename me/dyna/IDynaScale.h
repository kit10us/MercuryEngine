// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/dyna/Dyna.h>
#include <unify/V3.h>
#include <unify/TimeDelta.h>
#include <memory>

namespace me::dyna
{
	/// <summary>
	/// Given a delta, this performs a function of the delta resulting
	/// in a vector representing scale.
	/// </summary>
	class IDynaScale
	{
	public:
		typedef std::shared_ptr< IDynaScale > ptr;
		virtual ~IDynaScale() {}

		/// <summary>
		/// Returns vector representing scaling.
		/// </summary>
		virtual Result GetScale( unify::V3< float >& out, unify::TimeDelta delta ) = 0;
	};
}