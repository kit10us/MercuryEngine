// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <unify/Exception.h>

namespace me::exception
{
	/// <summary>
	/// A value was used that is not supported.
	/// </summary>
	class UnsupportedValue : public unify::Exception
	{
	public:
		UnsupportedValue( std::string type, std::string value )
			: unify::Exception( "Unsupported value \"" + value + "\" of type " + type + "!" )
		{
		}

		UnsupportedValue( std::string value )
			: unify::Exception( "Unsupported value \"" + value + "\"!" )
		{
		}
	};
}