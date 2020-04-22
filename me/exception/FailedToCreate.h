// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <unify/Exception.h>

namespace me::exception
{
	/// <summary>
	/// A failure during the creation process of something, where
	/// the creation was incomplete.
	/// </summary>
	class FailedToCreate : public unify::Exception
	{
	public:
		FailedToCreate( std::string error )
			: unify::Exception( error.c_str() )
		{
		}
	};
}