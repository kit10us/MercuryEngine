// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/dyna/Dyna.h>
#include <unify/TimeDelta.h>
#include <memory>

namespace me::dyna
{
	/// <summary>
	/// Produces a variable delta from a uniform linear delta.
	/// For example, if can take a unform delta, then
	/// output a different delta, which could be used
	/// to produce effects such as curves, binary snap-to
	/// changes, and such.
	/// </summary>
	class IDynaDelta
	{
	public:
		typedef std::shared_ptr< IDynaDelta > ptr;
		virtual ~IDynaDelta() {}

		/// <summary>
		/// Returns a delta result from a function of an
		/// input delta.
		/// </summary>
		virtual Result GetDelta( unify::TimeDelta& delta ) = 0;
	};
}