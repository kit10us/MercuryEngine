// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/exception/Exception.h>
#include <string>

namespace me
{
	namespace exception
	{
		class UnsupportedValue : public unify::Exception
		{
		public:
			UnsupportedValue( const std::string & type, const std::string & value )
				: unify::Exception( "Unsupported value \"" + value + "\" of type " + type + "!" )
			{
			}
			UnsupportedValue( const std::string & value )
				: unify::Exception( "Unsupported value \"" + value + "\"!" )
			{
			}
		};
	}
}