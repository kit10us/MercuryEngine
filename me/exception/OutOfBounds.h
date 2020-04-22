// Copyright (c) 2006 - 2014 Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <unify/Exception.h>

namespace me::exception
{
	/// <summary>
	/// There was an attempt to access an element of a set that
	/// is outside of the set's bounds.
	/// </summary>
	class OutOfBounds : public unify::Exception
	{
	public:
		OutOfBounds( std::string error )
			: unify::Exception( error )
		{
		}
	};
}