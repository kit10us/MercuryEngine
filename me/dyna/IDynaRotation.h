// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/dyna/Dyna.h>
#include <unify/Quaternion.h>
#include <unify/TimeDelta.h>
#include <memory>

namespace me
{
	namespace dyna
	{
		/// <summary>
		/// Given an input delta, this produces a function of a delta.
		/// </summary>
		class IDynaRotation
		{
		public:
			typedef std::shared_ptr< IDynaRotation > ptr;
			virtual ~IDynaRotation() {}

			/// <summary>
			/// Returns a quaternion result from a function of a delta.
			/// </summary>
			virtual Result GetRotation( unify::Quaternion & out, unify::TimeDelta delta ) = 0;
		};
	}
}