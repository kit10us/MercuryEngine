// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <unify/Exception.h>

namespace me::exception
{
	/// <summary>
	/// A failure in attempting to lock a resource for
	/// access.
	/// </summary>
	class FailedToLock : public unify::Exception
	{
	public:
		FailedToLock( std::string error )
			: unify::Exception( error )
		{
		}
	};
}