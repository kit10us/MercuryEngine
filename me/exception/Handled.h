// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <unify/Exception.h>

namespace me::exception
{
	/// <summary>
	/// An exception occured that has already been dealt with
	/// in such a way that there is no further need to respond
	/// to the specificity of the originating exception.
	/// </summary>
	class Handled : public unify::Exception
	{
	public:
		Handled( std::string what )
			: Exception( what )
		{
		}
	};
}